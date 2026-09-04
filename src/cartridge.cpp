#include "cartridge.h"
#include "files.h"

Cartridge::Cartridge(std::string filename){
    auto romData = readBytes(filename);
}