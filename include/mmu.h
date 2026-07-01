// Not included in original gameboy but this helps
#ifndef MMU_H
#define MMU_H

#include <cstdint>
#include "memory.h"
#include "address.h"

class MMU{
public: 
    MMU();

    uint8_t readByte(const Address addr) const;
    uint16_t readWord(const Address addr) const;

    void writeByte(const Address addr, const uint8_t byte);
    void writeWord(const Address addr, const uint16_t word);

private:
    Memory memory;
};


#endif