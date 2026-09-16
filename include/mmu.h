#ifndef MMU_H
#define MMU_H

#include <array>
#include <cstdint>
#include "address.h"
#include "cartridge.h"
#include "interrupt.h"
#include "clock.h"
#include "input.h"
#include "ppu.h"

class MMU{
public: 
    MMU(Cartridge& cartridge);

    uint8_t readByte(const Address addr) const;

    void writeByte(const Address addr, const uint8_t byte);
    void writeWord(const Address addr, const uint16_t word);

    void requestInterrupt(Interrupt interrupt);
    void clearInterrupt(Interrupt interrupt);
    uint8_t pendingInterrupts() const;

    void buttonPressed(Button button);
    void buttonReleased(Button button);

    void tick(uint8_t mCycles);

private:
    Cartridge& cartridge;
    Clock clock;
    Input input;
    PPU ppu;

    uint8_t readIO(uint16_t loc) const;
    void writeIO(uint16_t loc, uint8_t byte);

    std::array<uint8_t, 0x2000> vram{};
    std::array<uint8_t, 0x2000> wram{};
    std::array<uint8_t, 0x00A0> oam{};
    std::array<uint8_t, 0x0080> io{};
    std::array<uint8_t, 0x007F> hram{};

    uint8_t interruptEnable = 0;

    bool bootROMLoaded = true; // Todo true for now
};


#endif