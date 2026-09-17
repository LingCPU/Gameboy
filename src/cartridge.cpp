#include "cartridge.h"
#include "files.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <exception>

namespace{
    constexpr std::size_t romBankSize = 0x4000;
    constexpr std::size_t ramBankSize = 0x2000;
    constexpr std::size_t titleLength = 11;
    constexpr std::size_t minimumHeaderSize = 0x150;

    std::size_t ramSizeFromCode(uint8_t code){
        switch(code){
            case 0x00: return 0;
            case 0x01: return 0x0800;   // 2 KiB
            case 0x02: return 0x2000;   // 8 KiB
            case 0x03: return 0x8000;   // 32 KiB
            case 0x04: return 0x20000;  // 128 KiB
            case 0x05: return 0x10000;  // 64 KiB
            default: throw std::runtime_error("Unsupported cartridge RAM size code");
        }
    }

    std::string titleFromROM(const std::vector<uint8_t>& rom){
        std::string result;
        result.reserve(titleLength);

        for(std::size_t i = 0; i < titleLength; ++i){
            const uint8_t character = rom[cartridgeHeader::title + i];
            if(character == 0) break;
            result.push_back(static_cast<char>(character));
        }

        return result;
    }

    std::size_t wrapBank(std::size_t bank, std::size_t byteSize, std::size_t bankSize){
        if(byteSize == 0) return 0;
        const std::size_t banks = std::max<std::size_t>(1, (byteSize + bankSize - 1) / bankSize);
        return bank % banks;
    }

    std::size_t wrappedRAMAddress(std::size_t bank, uint16_t address, std::size_t ramSize){
        if(ramSize == 0) return 0;
        const std::size_t offset = bank * ramBankSize + (address - 0xA000);
        return offset % ramSize;
    }
}

class MemoryBankController{
public:
    MemoryBankController(std::vector<uint8_t> romData, std::size_t ramSize)
        : rom(std::move(romData)), ram(ramSize, 0){}

    virtual ~MemoryBankController() = default;

    virtual uint8_t read(Address address) const = 0;
    virtual void write(Address address, uint8_t value) = 0;

    const std::vector<uint8_t>& ramData() const{
        return ram;
    }

    void loadRAMData(const std::vector<uint8_t>& data){
        if(data.size() != ram.size()){
            throw std::runtime_error(
                "Save RAM size mismatch: read " + std::to_string(data.size())
                + " bytes, expected " + std::to_string(ram.size())
            );
        }
        ram = data;
    }

    virtual std::vector<uint8_t> rtcData() const{
        return {};
    }

    virtual void loadRTCData(const std::vector<uint8_t>& data){
        if(!data.empty()) throw std::runtime_error("RTC data supplied to a cartridge without an RTC");
    }

protected:
    uint8_t readROMBank(std::size_t bank, uint16_t addressInBank) const{
        if(rom.empty()) return 0xFF;
        bank = wrapBank(bank, rom.size(), romBankSize);
        const std::size_t index = bank * romBankSize + addressInBank;
        return index < rom.size() ? rom[index] : 0xFF;
    }

    uint8_t readRAMBank(std::size_t bank, uint16_t address) const{
        if(ram.empty()) return 0xFF;
        return ram[wrappedRAMAddress(bank, address, ram.size())];
    }

    void writeRAMBank(std::size_t bank, uint16_t address, uint8_t value){
        if(ram.empty()) return;
        ram[wrappedRAMAddress(bank, address, ram.size())] = value;
    }

    std::vector<uint8_t> rom;
    std::vector<uint8_t> ram;
};

namespace{
    class NoMBC final : public MemoryBankController{
    public:
        NoMBC(std::vector<uint8_t> romData, std::size_t ramSize)
            : MemoryBankController(std::move(romData), ramSize){}

        uint8_t read(Address address) const override{
            const uint16_t location = address.value();
            if(location <= 0x7FFF) return location < rom.size() ? rom[location] : 0xFF;
            if(location >= 0xA000 && location <= 0xBFFF) return readRAMBank(0, location);
            return 0xFF;
        }

        void write(Address address, uint8_t value) override{
            const uint16_t location = address.value();
            if(location >= 0xA000 && location <= 0xBFFF) writeRAMBank(0, location, value);
        }
    };

    class MBC1 final : public MemoryBankController{
    public:
        MBC1(std::vector<uint8_t> romData, std::size_t ramSize)
            : MemoryBankController(std::move(romData), ramSize){}

        uint8_t read(Address address) const override{
            const uint16_t location = address.value();

            if(location <= 0x3FFF){
                const std::size_t bank = bankingMode ? (upperBankBits << 5) : 0;
                return readROMBank(bank, location);
            }

            if(location <= 0x7FFF){
                const std::size_t bank = (upperBankBits << 5) | lowerROMBankBits;
                return readROMBank(bank, location - 0x4000);
            }

            if(location >= 0xA000 && location <= 0xBFFF){
                if(!ramEnabled || ram.empty()) return 0xFF;
                const std::size_t bank = bankingMode ? upperBankBits : 0;
                return readRAMBank(bank, location);
            }

            return 0xFF;
        }

        void write(Address address, uint8_t value) override{
            const uint16_t location = address.value();

            if(location <= 0x1FFF){
                ramEnabled = (value & 0x0F) == 0x0A;
                return;
            }

            if(location <= 0x3FFF){
                lowerROMBankBits = value & 0x1F;
                if(lowerROMBankBits == 0) lowerROMBankBits = 1;
                return;
            }

            if(location <= 0x5FFF){
                upperBankBits = value & 0x03;
                return;
            }

            if(location <= 0x7FFF){
                bankingMode = (value & 0x01) != 0;
                return;
            }

            if(location >= 0xA000 && location <= 0xBFFF && ramEnabled){
                const std::size_t bank = bankingMode ? upperBankBits : 0;
                writeRAMBank(bank, location, value);
            }
        }

    private:
        bool ramEnabled = false;
        uint8_t lowerROMBankBits = 1;
        uint8_t upperBankBits = 0;
        bool bankingMode = false;
    };

    class MBC2 final : public MemoryBankController{
    public:
        explicit MBC2(std::vector<uint8_t> romData)
            : MemoryBankController(std::move(romData), 0x0200){}

        uint8_t read(Address address) const override{
            const uint16_t location = address.value();

            if(location <= 0x3FFF) return readROMBank(0, location);

            if(location <= 0x7FFF) return readROMBank(romBank, location - 0x4000);
            
            if(location >= 0xA000 && location <= 0xBFFF){
                if(!ramEnabled) return 0xFF;
                const std::size_t index = (location - 0xA000) & 0x01FF;
                return static_cast<uint8_t>(0xF0 | (ram[index] & 0x0F));
            }

            return 0xFF;
        }

        void write(Address address, uint8_t value) override{
            const uint16_t location = address.value();

            if(location <= 0x3FFF){
                if((location & 0x0100) == 0) ramEnabled = (value & 0x0F) == 0x0A;
                else{
                    romBank = value & 0x0F;
                    if(romBank == 0) romBank = 1;
                }
                return;
            }

            if(location >= 0xA000 && location <= 0xBFFF && ramEnabled){
                const std::size_t index = (location - 0xA000) & 0x01FF;
                ram[index] = value & 0x0F;
            }
        }

    private:
        bool ramEnabled = false;
        uint8_t romBank = 1;
    };

    class MBC3 final : public MemoryBankController{
    public:
        MBC3(std::vector<uint8_t> romData, std::size_t ramSize, bool timerPresent)
            : MemoryBankController(std::move(romData), ramSize),
            hasTimer(timerPresent),
            lastRTCUpdate(std::chrono::steady_clock::now()){}

        uint8_t read(Address address) const override{
            const uint16_t location = address.value();

            if(location <= 0x3FFF) return readROMBank(0, location);
            if(location <= 0x7FFF) return readROMBank(romBank, location - 0x4000);
            
            if(location >= 0xA000 && location <= 0xBFFF){
                if(!ramTimerEnabled) return 0xFF;
                if(selection <= 0x07) return readRAMBank(selection, location);
                if(selection >= 0x08 && selection <= 0x0C && hasTimer) return readRTC(selection);
            }

            return 0xFF;
        }

        void write(Address address, uint8_t value) override{
            const uint16_t location = address.value();

            if(location <= 0x1FFF){
                ramTimerEnabled = (value & 0x0F) == 0x0A;
                return;
            }

            if(location <= 0x3FFF){
                romBank = value & 0x7F;
                if(romBank == 0) romBank = 1;
                return;
            }

            if(location <= 0x5FFF){
                selection = value;
                return;
            }

            if(location <= 0x7FFF){
                if(lastLatchWrite == 0 && value == 1 && hasTimer){
                    syncRTC();
                    latchedRTC = rtc;
                    rtcLatched = true;
                }
                lastLatchWrite = value;
                return;
            }

            if(location >= 0xA000 && location <= 0xBFFF && ramTimerEnabled){
                if(selection <= 0x07) writeRAMBank(selection, location, value);
                else if(selection >= 0x08 && selection <= 0x0C && hasTimer) writeRTC(selection, value); 
            }
        }
        
        std::vector<uint8_t> rtcData() const override{
            if(!hasTimer) return {};

            syncRTC();

            std::vector<uint8_t> data;
            data.reserve(19);
            data.push_back('G');
            data.push_back('B');
            data.push_back('R');
            data.push_back('T');
            data.push_back(1); // format version
            data.push_back(rtc.seconds);
            data.push_back(rtc.minutes);
            data.push_back(rtc.hours);
            data.push_back(static_cast<uint8_t>(rtc.days & 0xFF));
            data.push_back(static_cast<uint8_t>((rtc.days >> 8) & 0x01));
            data.push_back(static_cast<uint8_t>((rtc.halt ? 0x01 : 0x00) | (rtc.carry ? 0x02 : 0x00)));

            const auto now = std::chrono::system_clock::now();
            const auto unixSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
            const uint64_t timestamp = static_cast<uint64_t>(unixSeconds);
            for(unsigned shift = 0; shift < 64; shift += 8){
                data.push_back(static_cast<uint8_t>((timestamp >> shift) & 0xFF));
            }

            return data;
        }

        void loadRTCData(const std::vector<uint8_t>& data) override{
            if(!hasTimer){
                MemoryBankController::loadRTCData(data);
                return;
            }

            constexpr std::size_t rtcSaveSize = 19;
            if(data.size() != rtcSaveSize){
                throw std::runtime_error(
                    "RTC save size mismatch: read " + std::to_string(data.size())
                    + " bytes, expected " + std::to_string(rtcSaveSize)
                );
            }

            if(data[0] != 'G' || data[1] != 'B' || data[2] != 'R' || data[3] != 'T' || data[4] != 1){
                throw std::runtime_error("Invalid or unsupported RTC save file");
            }

            rtc.seconds = static_cast<uint8_t>(data[5] % 60);
            rtc.minutes = static_cast<uint8_t>(data[6] % 60);
            rtc.hours = static_cast<uint8_t>(data[7] % 24);
            rtc.days = static_cast<uint16_t>(data[8] | ((data[9] & 0x01) << 8));
            rtc.halt = (data[10] & 0x01) != 0;
            rtc.carry = (data[10] & 0x02) != 0;
            rtcLatched = false;

            uint64_t savedTimestamp = 0;
            for(unsigned byte = 0; byte < 8; ++byte){
                savedTimestamp |= static_cast<uint64_t>(data[11 + byte]) << (byte * 8);
            }

            lastRTCUpdate = std::chrono::steady_clock::now();

            if(!rtc.halt){
                const auto now = std::chrono::system_clock::now();
                const auto unixSeconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
                const uint64_t currentTimestamp = unixSeconds > 0 ? static_cast<uint64_t>(unixSeconds) : 0;
                if(currentTimestamp > savedTimestamp) addSeconds(currentTimestamp - savedTimestamp);
            }
        }

    private:
        struct RTC{
            uint8_t seconds = 0;
            uint8_t minutes = 0;
            uint8_t hours = 0;
            uint16_t days = 0;
            bool halt = false;
            bool carry = false;
        };

        void syncRTC() const{
            const auto now = std::chrono::steady_clock::now();
            if(rtc.halt){
                lastRTCUpdate = now;
                return;
            }

            const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastRTCUpdate).count();
            if(elapsed <= 0) return;

            lastRTCUpdate += std::chrono::seconds(elapsed);
            addSeconds(static_cast<uint64_t>(elapsed));
        }

        void addSeconds(uint64_t secondsToAdd) const{
            uint64_t total = rtc.seconds;
            total += static_cast<uint64_t>(rtc.minutes) * 60;
            total += static_cast<uint64_t>(rtc.hours) * 60 * 60;
            total += static_cast<uint64_t>(rtc.days) * 24 * 60 * 60;
            total += secondsToAdd;

            constexpr uint64_t secondsPerDay = 24 * 60 * 60;
            constexpr uint64_t secondsPerCycle = 512 * secondsPerDay;

            if(total >= secondsPerCycle){
                rtc.carry = true;
                total %= secondsPerCycle;
            }

            rtc.days = static_cast<uint16_t>(total / secondsPerDay);
            total %= secondsPerDay;
            rtc.hours = static_cast<uint8_t>(total / 3600);
            total %= 3600;
            rtc.minutes = static_cast<uint8_t>(total / 60);
            rtc.seconds = static_cast<uint8_t>(total % 60);
        }

        uint8_t readRTC(uint8_t reg) const{
            syncRTC();
            const RTC& visible = rtcLatched ? latchedRTC : rtc;

            switch(reg){
                case 0x08: return visible.seconds;
                case 0x09: return visible.minutes;
                case 0x0A: return visible.hours;
                case 0x0B: return static_cast<uint8_t>(visible.days & 0xFF);
                case 0x0C:
                    return static_cast<uint8_t>(((visible.days >> 8) & 0x01)
                        | (visible.halt ? 0x40 : 0)
                        | (visible.carry ? 0x80 : 0));
                default: return 0xFF;
            }
        }

        void writeRTC(uint8_t reg, uint8_t value){
            syncRTC();
            switch(reg){
                case 0x08:
                    rtc.seconds = static_cast<uint8_t>(value % 60);
                    break;
                case 0x09:
                    rtc.minutes = static_cast<uint8_t>(value % 60);
                    break;
                case 0x0A:
                    rtc.hours = static_cast<uint8_t>(value % 24);
                    break;
                case 0x0B:
                    rtc.days = static_cast<uint16_t>((rtc.days & 0x0100) | value);
                    break;
                case 0x0C: {
                    const bool wasHalted = rtc.halt;
                    rtc.days = static_cast<uint16_t>((rtc.days & 0x00FF) | ((value & 0x01) << 8));
                    rtc.halt = (value & 0x40) != 0;
                    rtc.carry = (value & 0x80) != 0;
                    if(wasHalted != rtc.halt) lastRTCUpdate = std::chrono::steady_clock::now();
                    break;
                }
                default:
                    break;
            }
        }

        bool ramTimerEnabled = false;
        uint8_t romBank = 1;
        uint8_t selection = 0;
        uint8_t lastLatchWrite = 0xFF;
        bool hasTimer = false;

        mutable RTC rtc{};
        mutable RTC latchedRTC{};
        mutable bool rtcLatched = false;
        mutable std::chrono::steady_clock::time_point lastRTCUpdate;
    };

    class MBC5 final : public MemoryBankController{
    public:
        MBC5(std::vector<uint8_t> romData, std::size_t ramSize, bool rumblePresent)
            : MemoryBankController(std::move(romData), ramSize), hasRumble(rumblePresent){}

        uint8_t read(Address address) const override{
            const uint16_t location = address.value();

            if(location <= 0x3FFF) return readROMBank(0, location);
            
            if(location <= 0x7FFF) return readROMBank(romBank, location - 0x4000);
            
            if(location >= 0xA000 && location <= 0xBFFF){
                if(!ramEnabled) return 0xFF;
                return readRAMBank(ramBank, location);
            }

            return 0xFF;
        }

        void write(Address address, uint8_t value) override{
            const uint16_t location = address.value();

            if(location <= 0x1FFF){
                ramEnabled = (value & 0x0F) == 0x0A;
                return;
            }

            if(location <= 0x2FFF){
                romBank = static_cast<uint16_t>((romBank & 0x0100) | value);
                return;
            }

            if(location <= 0x3FFF){
                romBank = static_cast<uint16_t>((romBank & 0x00FF) | ((value & 0x01) << 8));
                return;
            }

            if(location <= 0x5FFF){
                if(hasRumble){
                    rumbleEnabled = (value & 0x08) != 0;
                    ramBank = value & 0x07;
                } else ramBank = value & 0x0F;
                
                return;
            }

            if(location >= 0xA000 && location <= 0xBFFF && ramEnabled) writeRAMBank(ramBank, location, value);
        }

    private:
        bool ramEnabled = false;
        uint16_t romBank = 1;
        uint8_t ramBank = 0;
        bool hasRumble = false;
        bool rumbleEnabled = false;
    };

    struct CartridgeDescription{
        CartridgeType type;
        bool hasTimer = false;
        bool hasRumble = false;
        bool hasBattery = false;
    };

    CartridgeDescription describeCartridge(uint8_t typeCode){
        switch(typeCode){
            case 0x00:
            case 0x08: return {CartridgeType::ROMOnly};
            case 0x09: return {CartridgeType::ROMOnly, false, false, true};

            case 0x01:
            case 0x02: return {CartridgeType::MBC1};
            case 0x03: return {CartridgeType::MBC1, false, false, true};

            case 0x05: return {CartridgeType::MBC2};
            case 0x06: return {CartridgeType::MBC2, false, false, true};

            case 0x0F: return {CartridgeType::MBC3, true, false, true};
            case 0x10: return {CartridgeType::MBC3, true, false, true};
            case 0x11:
            case 0x12: return {CartridgeType::MBC3, false, false};
            case 0x13: return {CartridgeType::MBC3, false, false, true};

            case 0x19:
            case 0x1A: return {CartridgeType::MBC5, false, false};
            case 0x1B: return {CartridgeType::MBC5, false, false, true};
            case 0x1C:
            case 0x1D: return {CartridgeType::MBC5, false, true};
            case 0x1E: return {CartridgeType::MBC5, false, true, true};

            default:
                throw std::runtime_error("Unsupported cartridge type code: " + std::to_string(typeCode));
        }
    }

    std::vector<uint8_t> convertROMBytes(const std::vector<char>& bytes){
        std::vector<uint8_t> result;
        result.reserve(bytes.size());
        for(char byte : bytes){
            result.push_back(static_cast<uint8_t>(static_cast<unsigned char>(byte)));
        }
        return result;
    }
}

Cartridge::Cartridge(const std::string& filename){
    romFilename = filename;
    initialize(convertROMBytes(readBytes(filename)));
    loadPersistentData();
}

Cartridge::Cartridge(std::vector<uint8_t> romData){
    initialize(std::move(romData));
}

Cartridge::~Cartridge(){
    if(!batteryBacked || romFilename.empty()) return;

    try{
        save();
    } catch(const std::exception&){
        // Destructors must not throw. Explicit calls to save() still report I/O errors.
    }
}

void Cartridge::initialize(std::vector<uint8_t> romData){
    if(romData.size() < minimumHeaderSize) throw std::runtime_error("ROM is too small to contain a valid Game Boy cartridge header");

    loadedROMSize = romData.size();
    titleText = titleFromROM(romData);
    cartridgeTypeCode = romData[cartridgeHeader::cartridgeType];
    headerROMSizeCode = romData[cartridgeHeader::romSize];
    headerRAMSizeCode = romData[cartridgeHeader::ramSize];

    const CartridgeDescription description = describeCartridge(cartridgeTypeCode);
    cartridgeType = description.type;
    batteryBacked = description.hasBattery;

    std::size_t ramSize = ramSizeFromCode(headerRAMSizeCode);

    switch(cartridgeType){
        case CartridgeType::ROMOnly:
            controller = std::make_shared<NoMBC>(std::move(romData), ramSize);
            break;
        case CartridgeType::MBC1:
            controller = std::make_shared<MBC1>(std::move(romData), ramSize);
            break;
        case CartridgeType::MBC2:
            controller = std::make_shared<MBC2>(std::move(romData));
            break;
        case CartridgeType::MBC3:
            controller = std::make_shared<MBC3>(std::move(romData), ramSize, description.hasTimer);
            break;
        case CartridgeType::MBC5:
            controller = std::make_shared<MBC5>(std::move(romData), ramSize, description.hasRumble);
            break;
    }
}

uint8_t Cartridge::read(Address address) const{
    return controller->read(address);
}

void Cartridge::write(Address address, uint8_t value){
    controller->write(address, value);

    if(batteryBacked && !romFilename.empty()
        && address.value() <= 0x1FFF
        && (value & 0x0F) != 0x0A){
        save();
    }
}

void Cartridge::loadPersistentData(){
    if(!batteryBacked || romFilename.empty()) return;

    const std::string ramFilename = saveFilename();
    if(fileExists(ramFilename)){
        controller->loadRAMData(convertROMBytes(readBytes(ramFilename)));
    }

    const std::string rtcFilename = romFilename + ".rtc";
    if(fileExists(rtcFilename)){
        controller->loadRTCData(convertROMBytes(readBytes(rtcFilename)));
    }
}

void Cartridge::save() const{
    if(!batteryBacked || romFilename.empty()) return;

    const std::vector<uint8_t>& ram = controller->ramData();
    if(!ram.empty()) writeBytes(saveFilename(), ram);

    const std::vector<uint8_t> rtc = controller->rtcData();
    if(!rtc.empty()) writeBytes(romFilename + ".rtc", rtc);
}

std::string Cartridge::gameTitle() const{
    return titleText;
}

CartridgeType Cartridge::type() const{
    return cartridgeType;
}

uint8_t Cartridge::typeCode() const{
    return cartridgeTypeCode;
}

uint8_t Cartridge::romSizeCode() const{
    return headerROMSizeCode;
}

uint8_t Cartridge::ramSizeCode() const{
    return headerRAMSizeCode;
}

std::size_t Cartridge::romSizeBytes() const{
    return loadedROMSize;
}

std::size_t Cartridge::ramSizeBytes() const{
    return controller->ramData().size();
}

bool Cartridge::hasBattery() const{
    return batteryBacked;
}

std::string Cartridge::saveFilename() const{
    return romFilename.empty() ? std::string{} : romFilename + ".sav";
}

const std::vector<uint8_t>& Cartridge::ramData() const{
    return controller->ramData();
}