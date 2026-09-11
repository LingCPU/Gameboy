#include "mmu.h"
#include "boot.h"

MMU::MMU(Cartridge& inCartridge) : cartridge(inCartridge){

}

uint8_t MMU::readByte(const Address addr) const{
    if(addr.inRange()){
        if(bootROMLoaded){
            return bootDMG[addr.value()];
        }
    }
}

void MMU::writeByte(const Address addr, const uint8_t byte){
}

void MMU::writeWord(const Address addr, const uint16_t word){
}