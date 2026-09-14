#include "gameboy.h"
#include <utility>

Gameboy::Gameboy(Cartridge inCartridge) 
: cartridge(std::move(inCartridge)), 
  mmu(cartridge), 
  cpu(mmu){
    
}

void Gameboy::run(){
    while(true){
        const uint8_t cycles = cpu.tick();
        mmu.tick(cycles);
    }
}

void Gameboy::buttonPressed(Button button){
    mmu.buttonPressed(button);
}

void Gameboy::buttonReleased(Button button){
    mmu.buttonReleased(button);
}