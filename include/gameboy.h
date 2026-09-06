#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"

class Gameboy{
public: 
    Gameboy(Cartridge cartridge);

    void run();

private:
    MMU mmu;
    CPU cpu;
};

#endif