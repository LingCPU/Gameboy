#ifndef PPU_H
#define PPU_H

#include <cstdint>

enum class PPUMode : uint8_t{
    HBlank        = 0,
    VBlank        = 1,
    OAMScan       = 2,
    PixelTransfer = 3
};

class PPU{
public:
    PPU();

    void tick(uint8_t mCycles); // M-Cycles to T-Cycles

    uint8_t readLCDC() const;
    uint8_t readSTAT() const;
    uint8_t readLY() const;
    uint8_t readLYC() const;

    void writeLCDC(uint8_t val);
    void writeSTAT(uint8_t val);
    void writeLYC(uint8_t val);

    bool consumeVBlankInterrupt();
    bool consumeSTATInterrupt();

private:
    static constexpr uint16_t oamTCycles = 80;
    static constexpr uint16_t transferTCycles = 172;
    static constexpr uint16_t hBlankTCycles = 204;
    static constexpr uint16_t scanlineTCycles = 456;

    uint8_t lcdc = 0;
    uint8_t statInterruptEnable = 0;
    uint8_t ly = 0;
    uint8_t lyc = 0;

    PPUMode mode = PPUMode::HBlank;
    uint16_t modeTCycleCounter = 0;

    bool statInterruptLine = false;
    bool vBlankInterruptRequested = false;
    bool statInterruptRequested = false;

    bool lcdEnabled() const;
    uint16_t currentModeDuration() const;
    void advanceMode();
    void updateSTATInterruptLine();
};

#endif