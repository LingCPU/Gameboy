#include "gameboy.h"
#include <utility>

Gameboy::Gameboy(Cartridge inCartridge) 
: cartridge(std::move(inCartridge)), 
  mmu(cartridge), 
  cpu(mmu){
    
}

void Gameboy::run(){
    while(true){
        cpu.tick();
    }
}