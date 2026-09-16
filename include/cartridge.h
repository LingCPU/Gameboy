#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "address.h"

namespace cartridgeHeader{
    inline constexpr std::size_t entryPoint = 0x100;
    inline constexpr std::size_t logo = 0x104;
    inline constexpr std::size_t title = 0x134;
    inline constexpr std::size_t manufacturerCode = 0x13F;
    inline constexpr std::size_t cgbFlag = 0x143;
    inline constexpr std::size_t newLicenseCode = 0x144;
    inline constexpr std::size_t sgbFlag = 0x146;
    inline constexpr std::size_t cartridgeType = 0x147;
    inline constexpr std::size_t romSize = 0x148;
    inline constexpr std::size_t ramSize = 0x149;
    inline constexpr std::size_t destinationCode = 0x14A;
    inline constexpr std::size_t oldLicenseCode = 0x14B;
    inline constexpr std::size_t versionNumber = 0x14C;
    inline constexpr std::size_t headerChecksum = 0x14D;
    inline constexpr std::size_t globalChecksum = 0x14E;
}

enum class CartridgeType{
    ROMOnly,
    MBC1,
    MBC2,
    MBC3,
    MBC5
};

class MemoryBankController;

class Cartridge{
public:
    Cartridge(const std::string& filename);
    Cartridge(std::vector<uint8_t> romData);

    uint8_t read(const Address address) const;
    void write(Address address, uint8_t value);

    // Header info from Cartridge
    std::string gameTitle() const;
    CartridgeType type() const;
    uint8_t typeCode() const;
    uint8_t romSizeCode() const;
    uint8_t ramSizeCode() const;
    std::size_t romSizeBytes() const;
    std::size_t ramSizeBytes() const;

    const std::vector<uint8_t>& ramData() const;

private:
    void initialize(std::vector<uint8_t> romData);

    std::shared_ptr<MemoryBankController> controller;
    std::string titleText;
    CartridgeType cartridgeType = CartridgeType::ROMOnly;
    uint8_t cartridgeTypeCode = 0;
    uint8_t headerROMSizeCode = 0;
    uint8_t headerRAMSizeCode = 0;
    std::size_t loadedROMSize = 0;
};