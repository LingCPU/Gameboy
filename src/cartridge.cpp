#include "cartridge.h"
#include "files.h"

const int titleLength = 11;

Cartridge::Cartridge(std::string filename){
    data = readBytes(filename);
}

uint8_t Cartridge::read(const Address address) const{
    // todo
    return data[address.value()];
}

std::string Cartridge::gameTitle() const{
    char name[titleLength] = {0};
    for(int i = 0; i < titleLength; i++){
        name[i] = data[title + i];
    } 
    return std::string(name);
}