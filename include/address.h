#ifndef ADDRESS_H
#define ADDRESS_H
// Address class to represent 16-bit addresses in the Gameboy's memory map

#include <cstdint>
#include "register.h"

class Address{
public:
    explicit Address(uint16_t loc); // For direct 16-bit address values
    uint16_t value() const;
    bool inRange(Address low, Address high) const;

private:
    uint16_t addr = 0x0000; // Default to 0x0000
};

#endif