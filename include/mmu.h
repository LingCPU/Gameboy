// Not included in original gameboy but this helps
#ifndef MMU_H
#define MMU_H

#include <cstdint>
#include "address.h"
#include "cartridge.h"

class MMU{
public: 
    MMU(Cartridge& cartridge);

    uint8_t readByte(const Address addr) const;

    void writeByte(const Address addr, const uint8_t byte);
    void writeWord(const Address addr, const uint16_t word);

private:
    Cartridge& cartridge;
};


#endif