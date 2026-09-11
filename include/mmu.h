// Not included in original gameboy but this helps
#ifndef MMU_H
#define MMU_H

#include <array>
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

    std::array<uint8_t, 0x2000> vram{};
    std::array<uint8_t, 0x2000> wram{};
    std::array<uint8_t, 0x00A0> oam{};
    std::array<uint8_t, 0x0080> io{};
    std::array<uint8_t, 0x007F> hram{};

    uint8_t interruptEnable = 0;

    bool bootROMLoaded = true; // Todo true for now
};


#endif