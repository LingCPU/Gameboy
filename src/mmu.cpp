#include "mmu.h"
#include "boot.h"

namespace{
    constexpr uint16_t JOYP = 0xFF00;
    constexpr uint16_t DIV  = 0xFF04;
    constexpr uint16_t TIMA = 0xFF05;
    constexpr uint16_t TMA  = 0xFF06;
    constexpr uint16_t TAC  = 0xFF07;
}

MMU::MMU(Cartridge& inCartridge) 
: cartridge(inCartridge){
}

uint8_t MMU::readByte(const Address addr) const{
    uint16_t location = addr.value();
    if(bootROMLoaded && location <= 0x00FF) return bootDMG[location];
    if(location <= 0x7FFF) return cartridge.read(addr);
    if(location <= 0x9FFF) return vram[location - 0x8000];
    if(location <= 0xBFFF) return 0xFF; 
    if(location <= 0xDFFF) return wram[location - 0xC000];
    if(location <= 0xFDFF) return wram[location - 0xE000];
    if(location <= 0xFE9F) return oam[location - 0xFE00];
    if(location <= 0xFEFF) return 0xFF;
    if(location <= 0xFF7F) return readIO(location);
    if(location <= 0xFFFE) return hram[location - 0xFF80];
    
    return interruptEnable;
}

void MMU::writeByte(const Address addr, const uint8_t byte){
    uint16_t location = addr.value();
    if(location <= 0x7FFF) return;
    if(location <= 0x9FFF){
        vram[location - 0x8000] = byte;
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
        oam[location - 0xFE00] = byte;
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
}

uint8_t MMU::readIO(uint16_t location) const{
    switch(location){
        case JOYP: return input.read();
        case DIV:  return clock.readDIV();
        case TIMA: return clock.readTIMA();
        case TMA:  return clock.readTMA();
        case TAC:  return clock.readTAC();
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
        default:
            io[location - 0xFF00] = byte;
            if(location == 0xFF50 && byte != 0) bootROMLoaded = false;
            return;
    }
}