#include "mmu.h"

MMU::MMU(Cartridge& inCartridge) : cartridge(inCartridge){

}

uint8_t MMU::readByte(const Address addr) const{   
}

void MMU::writeByte(const Address addr, const uint8_t byte){
}

void MMU::writeWord(const Address addr, const uint16_t word){
}