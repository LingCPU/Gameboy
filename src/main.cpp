#include <cstdio>
#include "gameboy.h"
#include "cartridge.h"

int main(int argc, char* argv[]){
    std::string romName = argv[1];
    Cartridge cartridge(romName);
    Gameboy gameboy(cartridge);
    gameboy.run();
    
    return 0;
}