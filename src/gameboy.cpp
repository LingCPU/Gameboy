#include "gameboy.h"

Gameboy::Gameboy(Cartridge cartridge) : mmu(cartridge), cpu(mmu){
    
}

void Gameboy::run(){
    while(true){
        cpu.tick();
    }
}