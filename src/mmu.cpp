#include "mmu.h"
#include "boot.h"

namespace{
    constexpr uint16_t JOYP = 0xFF00;

    constexpr uint16_t DIV  = 0xFF04;
    constexpr uint16_t TIMA = 0xFF05;
    constexpr uint16_t TMA  = 0xFF06;
    constexpr uint16_t TAC  = 0xFF07;

    constexpr uint16_t LCDC = 0xFF40;
    constexpr uint16_t STAT = 0xFF41;
    constexpr uint16_t SCY  = 0xFF42;
    constexpr uint16_t SCX  = 0xFF43;
    constexpr uint16_t LY   = 0xFF44;
    constexpr uint16_t LYC  = 0xFF45;
    constexpr uint16_t DMA  = 0xFF46;
    constexpr uint16_t BGP  = 0xFF47;
    constexpr uint16_t OBP0 = 0xFF48;
    constexpr uint16_t OBP1 = 0xFF49;
    constexpr uint16_t WY   = 0xFF4A;
    constexpr uint16_t WX   = 0xFF4B;
}

MMU::MMU(Cartridge& inCartridge) 
: cartridge(inCartridge){
}

uint8_t MMU::readByte(const Address addr) const{
    uint16_t location = addr.value();
    if(bootROMLoaded && location <= 0x00FF) return bootDMG[location];
    if(location <= 0x7FFF) return cartridge.read(addr);
    if(location <= 0x9FFF) return ppu.readVRAM(location - 0x8000);
    if(location <= 0xBFFF) return 0xFF; 
    if(location <= 0xDFFF) return wram[location - 0xC000];
    if(location <= 0xFDFF) return wram[location - 0xE000];
    if(location <= 0xFE9F) return ppu.readOAM(location - 0xFE00);
    if(location <= 0xFEFF) return 0xFF;
    if(location <= 0xFF7F) return readIO(location);
    if(location <= 0xFFFE) return hram[location - 0xFF80];
    
    return interruptEnable;
}

void MMU::writeByte(const Address addr, const uint8_t byte){
    uint16_t location = addr.value();
    if(location <= 0x7FFF) return;
    if(location <= 0x9FFF){
        ppu.writeVRAM(location - 0x8000, byte);
        return;
    }
    if(location <= 0xBFFF) return;
    if(location <= 0xDFFF){
        wram[location - 0xC000] = byte;
        return;
    }
    if(location <= 0xFDFF){
        wram[location - 0xE000] = byte;
        return;
    }
    if(location <= 0xFE9F){
        ppu.writeOAM(location - 0xFE00, byte);
        return;
    }
    if(location <= 0xFEFF) return;
    if(location <= 0xFF7F){
        writeIO(location, byte);
        return;
    }
    if(location <= 0xFFFE){
        hram[location - 0xFF80] = byte;
        return;
    }
    interruptEnable = byte;
}

void MMU::writeWord(const Address addr, const uint16_t word){
    uint8_t low = static_cast<uint8_t>(word &0xFF);
    uint8_t high = static_cast<uint8_t>(word >> 8);
    writeByte(addr, low);
    writeByte(Address(static_cast<uint16_t>(addr.value() + 1)), high);
}

void MMU::requestInterrupt(Interrupt interrupt){
    io[interrupts::requestAddress - 0xFF00] |= interrupts::mask(interrupt);
}

void MMU::clearInterrupt(Interrupt interrupt){
    io[interrupts::requestAddress - 0xFF00] &= static_cast<uint8_t>(~interrupts::mask(interrupt));
}

uint8_t MMU::pendingInterrupts() const{
    const uint8_t interruptFlags = io[interrupts::requestAddress - 0xFF00];
    return static_cast<uint8_t>(interruptFlags & interruptEnable & 0x1F);
}

void MMU::buttonPressed(Button button){
    if(input.buttonPressed(button)) requestInterrupt(Interrupt::Joypad);
}

void MMU::buttonReleased(Button button){
    if(input.buttonReleased(button)) requestInterrupt(Interrupt::Joypad);
}

void MMU::tick(uint8_t mCycles){
    clock.addTime(mCycles);
    if(clock.consumeTimerInterrupt()) requestInterrupt(Interrupt::Timer);

    ppu.tick(mCycles);
    if(ppu.consumeVBlankInterrupt()) requestInterrupt(Interrupt::VBlank);
    if(ppu.consumeSTATInterrupt()) requestInterrupt(Interrupt::LCDStat);
}

const PPU::FrameBuffer& MMU::frameBuffer() const{
    return ppu.frameBuffer();
}

bool MMU::consumeFrameReady(){
    return ppu.consumeFrameReady();
}

uint8_t MMU::readIO(uint16_t location) const{
    switch(location){
        case JOYP: return input.read();

        case DIV:  return clock.readDIV();
        case TIMA: return clock.readTIMA();
        case TMA:  return clock.readTMA();
        case TAC:  return clock.readTAC();

        case LCDC: return ppu.readLCDC();
        case STAT: return ppu.readSTAT();
        case SCY:  return ppu.readSCY();
        case SCX:  return ppu.readSCX();
        case LY:   return ppu.readLY();
        case LYC:  return ppu.readLYC();
        case DMA:  return ppu.readDMA();
        case BGP:  return ppu.readBGP();
        case OBP0: return ppu.readOBP0();
        case OBP1: return ppu.readOBP1();
        case WY:   return ppu.readWY();
        case WX:   return ppu.readWX();

        default:   return io[location - 0xFF00];
    }
}

void MMU::writeIO(uint16_t location, uint8_t byte){
    switch(location){
        case JOYP:
            if(input.write(byte)) requestInterrupt(Interrupt::Joypad);
            return;

        case DIV:
            clock.writeDIV();
            return;
        case TIMA:
            clock.writeTIMA(byte);
            return;
        case TMA:
            clock.writeTMA(byte);
            return;
        case TAC:
            clock.writeTAC(byte);
            return;

        case LCDC:
            ppu.writeLCDC(byte);
            return;
        case STAT:
            ppu.writeSTAT(byte);
            return;
        case SCY:
            ppu.writeSCY(byte);
            return;
        case SCX:
            ppu.writeSCX(byte);
            return;
        case LY:
            // LY is generated by PPU, so writes are ignored
            return;
        case LYC:
            ppu.writeLYC(byte);
            return;
        case DMA:
            ppu.writeDMA(byte);
            dmaTransfer(byte);
            return;
        case BGP:
            ppu.writeBGP(byte);
            return;
        case OBP0:
            ppu.writeOBP0(byte);
            return;
        case OBP1:
            ppu.writeOBP1(byte);
            return;
        case WY:
            ppu.writeWY(byte);
            return;
        case WX:
            ppu.writeWX(byte);
            return;

        default:
            io[location - 0xFF00] = byte;
            if(location == 0xFF50 && byte != 0) bootROMLoaded = false;
            return;
    }
}

void MMU::dmaTransfer(uint8_t sourceHighByte){
    const uint16_t sourceBase = static_cast<uint16_t>(sourceHighByte) << 8;
    for(uint16_t offset = 0; offset < 0x00A0; ++offset){
        const uint16_t source = static_cast<uint16_t>(sourceBase + offset);
        ppu.writeOAMDMA(offset, readByte(Address(source)));
    }
}