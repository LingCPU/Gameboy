#include "mmu.h"
#include "boot.h"

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
    if(location <= 0xFF7F) return io[location - 0xFF00];
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
        io[location - 0xFF00] = byte;
        if(location == 0xFF50 && byte != 0) bootROMLoaded = false;
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