#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "input.h"

class Gameboy{
public: 
    Gameboy(Cartridge cartridge);

    void run();
    void buttonPressed(Button button);
    void buttonReleased(Button button);

private:
    Cartridge cartridge;
    MMU mmu;
    CPU cpu;
};

#endif