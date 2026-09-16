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
    static constexpr std::size_t screenWidth = 160;
    static constexpr std::size_t screenHeight = 144;
    using FrameBuffer = std::array<uint8_t, screenWidth * screenHeight>;
    
    PPU();

    void tick(uint8_t mCycles); // M-Cycles to T-Cycles

    uint8_t readVRAM(uint16_t offset) const;
    void writeVRAM(uint16_t offset, uint8_t value);

    uint8_t readOAM(uint16_t offset) const;
    void writeOAM(uint16_t offset, uint8_t value);

    void writeOAMDMA(uint16_t offset, uint8_t value);

    uint8_t readLCDC() const;
    uint8_t readSTAT() const;
    uint8_t readSCY() const;
    uint8_t readSCX() const;
    uint8_t readLY() const;
    uint8_t readLYC() const;
    uint8_t readDMA() const;
    uint8_t readBGP() const;
    uint8_t readOBP0() const;
    uint8_t readOBP1() const;
    uint8_t readWY() const;
    uint8_t readWX() const;

    void writeLCDC(uint8_t val);
    void writeSTAT(uint8_t val);
    void writeSCY(uint8_t value);
    void writeSCX(uint8_t value);
    void writeLYC(uint8_t val);
    void writeDMA(uint8_t value);
    void writeBGP(uint8_t value);
    void writeOBP0(uint8_t value);
    void writeOBP1(uint8_t value);
    void writeWY(uint8_t value);
    void writeWX(uint8_t value);

    const FrameBuffer& frameBuffer() const;
    bool consumeFrameReady();
    bool consumeVBlankInterrupt();
    bool consumeSTATInterrupt();

private:
    static constexpr uint16_t oamTCycles = 80;
    static constexpr uint16_t transferTCycles = 172;
    static constexpr uint16_t hBlankTCycles = 204;
    static constexpr uint16_t scanlineTCycles = 456;

    static constexpr std::size_t vramSize = 0x2000;
    static constexpr std::size_t oamSize = 0x00A0;

    std::array<uint8_t, vramSize> vram{};
    std::array<uint8_t, oamSize> oam{};
    FrameBuffer frame{};

    uint8_t lcdc = 0;
    uint8_t statInterruptEnable = 0;
    uint8_t scy = 0;
    uint8_t scx = 0;
    uint8_t ly = 0;
    uint8_t lyc = 0;
    uint8_t dma = 0;
    uint8_t bgp = 0;
    uint8_t obp0 = 0;
    uint8_t obp1 = 0;
    uint8_t wy = 0;
    uint8_t wx = 0;

    PPUMode mode = PPUMode::HBlank;
    uint16_t modeTCycleCounter = 0;

    bool statInterruptLine = false;
    bool vBlankInterruptRequested = false;
    bool statInterruptRequested = false;
    bool frameReady = false;

    bool lcdEnabled() const;
    bool backgroundWindowEnabled() const;
    bool spritesEnabled() const;
    bool tallSpritesEnabled() const;
    bool backgroundTileMapHigh() const;
    bool unsignedTileData() const;
    bool windowEnabled() const;
    bool windowTileMapHigh() const;

    bool cpuCanAccessVRAM() const;
    bool cpuCanAccessOAM() const;

    uint16_t currentModeDuration() const;
    void advanceMode();
    void updateSTATInterruptLine();

    void renderScanline();
    void renderBackgroundAndWindow(std::array<uint8_t, screenWidth>& backgroundColors);
    void renderSprites(const std::array<uint8_t, screenWidth>& backgroundColors);

    uint8_t tilePixel(uint8_t tileId, uint8_t pixelX, uint8_t pixelY, bool useUnsignedTileIds) const;
    static uint8_t paletteShade(uint8_t palette, uint8_t colorId);
};

#endif