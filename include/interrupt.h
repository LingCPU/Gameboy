#pragma once

#include <cstdint>

// Interrupts are ordered by hardware priority. The enum value is also the
// bit used by the IF (0xFF0F) and IE (0xFFFF) registers.
enum class Interrupt : uint8_t{
    VBlank  = 0, // VBlank      bit 0 -> 0x40
    LCDStat = 1, // LCD STAT    bit 1 -> 0x48
    Timer   = 2, // Timer       bit 2 -> 0x50
    Serial  = 3, // Serial      bit 3 -> 0x58
    Joypad  = 4  // Joypad      bit 4 -> 0x60
};

namespace interrupts{
    inline constexpr uint16_t requestAddress = 0xFF0F; // IF
    inline constexpr uint16_t enableAddress  = 0xFFFF; // IE

    inline constexpr uint8_t mask(Interrupt interrupt){
        return static_cast<uint8_t>(1u << static_cast<uint8_t>(interrupt));
    }

    inline constexpr uint16_t vector(Interrupt interrupt){
        return static_cast<uint16_t>(0x0040 + (static_cast<uint8_t>(interrupt) * 0x0008));
    }
}