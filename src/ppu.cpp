#include "ppu.h"
#include <algorithm>

namespace{
    constexpr uint16_t bgMapLowOffset = 0x1800;  // 0x9800 - 0x8000
    constexpr uint16_t bgMapHighOffset = 0x1C00; // 0x9C00 - 0x8000
    constexpr uint16_t signedTileBaseOffset = 0x1000; // 0x9000 - 0x8000
    constexpr uint8_t bytesPerTile = 16;
    constexpr uint8_t tilesPerMapRow = 32;
    constexpr uint8_t maxSpritesPerLine = 10;
    constexpr uint8_t spriteCount = 40;
    constexpr uint8_t bytesPerSprite = 4;
}

PPU::PPU(){
    frame.fill(0);
}

void PPU::tick(uint8_t mCycles){
    if(!lcdEnabled()) return;

    uint16_t remainingTCycles = static_cast<uint16_t>(mCycles) * 4;
    while(remainingTCycles > 0){
        const uint16_t duration = currentModeDuration();
        const uint16_t untilModeEnds = static_cast<uint16_t>(duration - modeTCycleCounter);
        const uint16_t step = std::min(remainingTCycles, untilModeEnds);

        modeTCycleCounter = static_cast<uint16_t>(modeTCycleCounter + step);
        remainingTCycles = static_cast<uint16_t>(remainingTCycles - step);

        if(modeTCycleCounter == duration){
            modeTCycleCounter = 0;
            advanceMode();
        }
    }
}

uint8_t PPU::readVRAM(uint16_t offset) const{
    if(offset >= vram.size() || !cpuCanAccessVRAM()) return 0xFF;
    return vram[offset];
}

void PPU::writeVRAM(uint16_t offset, uint8_t val){
    if(offset >= vram.size() || !cpuCanAccessVRAM()) return;
    vram[offset] = val;
}

uint8_t PPU::readOAM(uint16_t offset) const{
    if(offset >= oam.size() || !cpuCanAccessOAM()) return 0xFF;
    return oam[offset];
}

void PPU::writeOAM(uint16_t offset, uint8_t val){
    if(offset >= oam.size() || !cpuCanAccessOAM()) return;
    oam[offset] = val;
}

void PPU::writeOAMDMA(uint16_t offset, uint8_t val){
    if(offset >= oam.size()) return;
    oam[offset] = val;
}

uint8_t PPU::readLCDC() const{
    return lcdc;
}

uint8_t PPU::readSTAT() const{
    const uint16_t coincidence = (ly == lyc) ? 0x04 : 0x00;
    const uint16_t modeBits = static_cast<uint8_t>(mode);
    return static_cast<uint8_t>(0x80 | statInterruptEnable | coincidence | modeBits);
}

uint8_t PPU::readSCY() const{
    return scy;
}

uint8_t PPU::readSCX() const{
    return scx;
}

uint8_t PPU::readLY() const{
    return ly;
}

uint8_t PPU::readLYC() const{
    return lyc;
}

uint8_t PPU::readDMA() const{
    return dma;
}

uint8_t PPU::readBGP() const{
    return bgp;
}

uint8_t PPU::readOBP0() const{
    return obp0;
}

uint8_t PPU::readOBP1() const{
    return obp1;
}

uint8_t PPU::readWY() const{
    return wy;
}

uint8_t PPU::readWX() const{
    return wx;
}

void PPU::writeLCDC(uint8_t val){
    const bool wasEnabled = lcdEnabled();
    lcdc = val;
    const bool isEnabled = lcdEnabled();

    if(wasEnabled && !isEnabled){
        // turning off lcd resets ly and ppu in mode 0, while disabled tick does not advance ppu
        ly = 0;
        mode = PPUMode::HBlank;
        modeTCycleCounter = 0;
        statInterruptLine = false;
        frameReady = false;
        frame.fill(0);
        return;
    }

    if(!wasEnabled && isEnabled){
        // lcd begins visible frame timing sequence with OAM scan on line 0
        ly = 0;
        mode = PPUMode::OAMScan;
        modeTCycleCounter = 0;
        statInterruptLine = false;
        updateSTATInterruptLine();
    }
}

void PPU::writeSTAT(uint8_t val){
    // Only STAT bits 3-6 are writable. Mode and coincidence bits are always generated from the PPU's current state.
    statInterruptEnable = static_cast<uint8_t>(val & 0x78);
    updateSTATInterruptLine();
}

void PPU::writeSCY(uint8_t val){
    scy = val;
}

void PPU::writeSCX(uint8_t val){
    scx = val;
}

void PPU::writeLYC(uint8_t val){
    lyc = val;
    updateSTATInterruptLine();
}

void PPU::writeDMA(uint8_t val){
    dma = val;
}

void PPU::writeBGP(uint8_t val){
    bgp = val;
}

void PPU::writeOBP0(uint8_t val){
    obp0 = val;
}

void PPU::writeOBP1(uint8_t val){
    obp1 = val;
}

void PPU::writeWY(uint8_t val){
    wy = val;
}

void PPU::writeWX(uint8_t val){
    wx = val;
}

bool PPU::consumeFrameReady(){
    const bool ready = frameReady;
    frameReady = false;
    return ready;
}

const PPU::FrameBuffer& PPU::frameBuffer() const{
    return frame;
}
bool PPU::consumeVBlankInterrupt(){
    const bool requested = vBlankInterruptRequested;
    vBlankInterruptRequested = false;
    return requested;
}

bool PPU::consumeSTATInterrupt(){
    const bool requested = statInterruptRequested;
    statInterruptRequested = false;
    return requested;
}

bool PPU::lcdEnabled() const{
    return (lcdc.value() & 0x80) != 0;
}

bool PPU::backgroundWindowEnabled() const{
    return (lcdc.value() & 0x01) != 0;
}

bool PPU::spritesEnabled() const{
    return (lcdc.value() & 0x02) != 0;
}

bool PPU::tallSpritesEnabled() const{
    return (lcdc.value() & 0x04) != 0;
}

bool PPU::backgroundTileMapHigh() const{
    return (lcdc.value() & 0x08) != 0;
}

bool PPU::unsignedTileData() const{
    return (lcdc.value() & 0x10) != 0;
}

bool PPU::windowEnabled() const{
    return (lcdc.value() & 0x20) != 0;
}

bool PPU::windowTileMapHigh() const{
    return (lcdc.value() & 0x40) != 0;
}

bool PPU::cpuCanAccessVRAM() const{
    return !lcdEnabled() || mode != PPUMode::PixelTransfer;
}

bool PPU::cpuCanAccessOAM() const{
    return !lcdEnabled() || (mode != PPUMode::OAMScan && mode != PPUMode::PixelTransfer);
}

uint16_t PPU::currentModeDuration() const{
    switch(mode){
        case PPUMode::OAMScan:       return oamTCycles;
        case PPUMode::PixelTransfer: return transferTCycles;
        case PPUMode::HBlank:        return hBlankTCycles;
        case PPUMode::VBlank:        return scanlineTCycles;
    }
    return scanlineTCycles;
}

void PPU::advanceMode(){
    switch(mode){
        case PPUMode::OAMScan:
            mode = PPUMode::PixelTransfer;
            break;
        case PPUMode::PixelTransfer:
            renderScanline();
            mode = PPUMode::HBlank;
            break;

        case PPUMode::HBlank:
            ++ly;
            if(ly == 114){
                mode = PPUMode::VBlank;
                vBlankInterruptRequested = true;
            } else mode = PPUMode::OAMScan;
            break;
        case PPUMode::VBlank:
            if(ly == 153){
                ly = 0;
                mode = PPUMode::OAMScan;
            } else ++ly;
            break;
    }
    updateSTATInterruptLine();
}

void PPU::updateSTATInterruptLine(){
    bool newLine = false;

    if(lcdEnabled()){
        const bool coincidenceSource = (statInterruptEnable & 0x40) != 0 && ly == lyc;
        const bool oamSource = (statInterruptEnable & 0x20) != 0 && mode == PPUMode::OAMScan;
        const bool vBlankSource = (statInterruptEnable & 0x10) != 0 && mode == PPUMode::VBlank;
        const bool hBlankSource = (statInterruptEnable & 0x08) != 0 && mode == PPUMode::HBlank;

        newLine = coincidenceSource || oamSource || vBlankSource || hBlankSource;
    }

    if(newLine && !statInterruptLine) statInterruptRequested = true;
    statInterruptLine = newLine;
}

void PPU::renderScanline(){
    if(!lcdEnabled() || ly >= screenHeight) return;

    std::array<uint8_t, screenWidth> backgroundColors{};
    renderBackgroundAndWindow(backgroundColors);
    renderSprites(backgroundColors);
}

void PPU::renderBackgroundAndWindow(std::array<uint8_t, screenWidth>& backgroundColors){
    const std::size_t lineOffset = static_cast<std::size_t>(ly) * screenWidth;

    if(!backgroundWindowEnabled()){
        for(std::size_t x = 0; x < screenWidth; ++x){
            backgroundColors[x] = 0;
            frame[lineOffset + x] = 0;
        }
        return;
    }

    const uint16_t bgMapOffset = backgroundTileMapHigh() ? bgMapHighOffset : bgMapLowOffset;

    for(std::size_t x = 0; x < screenWidth; ++x){
        const uint8_t bgX = static_cast<uint8_t>(x + scx.value());
        const uint8_t bgY = static_cast<uint8_t>(ly.value() + scy.value());
        const uint8_t tileX = static_cast<uint8_t>(bgX / 8);
        const uint8_t tileY = static_cast<uint8_t>(bgY / 8);
        const uint16_t tileMapIndex = static_cast<uint16_t>(tileY * tilesPerMapRow + tileX);
        const uint8_t tileId = vram[bgMapOffset + tileMapIndex];
        const uint8_t colorId = tilePixel(tileId, static_cast<uint8_t>(bgX % 8),
                                          static_cast<uint8_t>(bgY % 8), unsignedTileData());

        backgroundColors[x] = colorId;
        frame[lineOffset + x] = paletteShade(bgp, colorId);
    }

    if(!windowEnabled() || ly < wy) return;

    const int windowLeft = static_cast<int>(wx.value()) - 7;
    if(windowLeft >= static_cast<int>(screenWidth)) return;

    const uint16_t windowMapOffset = windowTileMapHigh() ? bgMapHighOffset : bgMapLowOffset;
    const uint8_t windowY = static_cast<uint8_t>(ly.value() - wy.value());

    for(int screenX = std::max(windowLeft, 0); screenX < static_cast<int>(screenWidth); ++screenX){
        const uint8_t windowX = static_cast<uint8_t>(screenX - windowLeft);
        const uint8_t tileX = static_cast<uint8_t>(windowX / 8);
        const uint8_t tileY = static_cast<uint8_t>(windowY / 8);
        const uint16_t tileMapIndex = static_cast<uint16_t>(tileY * tilesPerMapRow + tileX);
        const uint8_t tileId = vram[windowMapOffset + tileMapIndex];
        const uint8_t colorId = tilePixel(tileId, static_cast<uint8_t>(windowX % 8),
                                          static_cast<uint8_t>(windowY % 8), unsignedTileData());

        const std::size_t x = static_cast<std::size_t>(screenX);
        backgroundColors[x] = colorId;
        frame[lineOffset + x] = paletteShade(bgp, colorId);
    }
}

void PPU::renderSprites(const std::array<uint8_t, screenWidth>& backgroundColors){
    if(!spritesEnabled()) return;

    struct SpriteOnLine{
        uint8_t index;
        uint8_t x;
    };

    std::array<SpriteOnLine, maxSpritesPerLine> sprites{};
    std::size_t spriteTotal = 0;
    const int spriteHeight = tallSpritesEnabled() ? 16 : 8;

    // The DMG considers only the first 10 OAM entries that intersect a scanline.
    for(uint8_t spriteIndex = 0; spriteIndex < spriteCount && spriteTotal < maxSpritesPerLine; ++spriteIndex){
        const std::size_t base = static_cast<std::size_t>(spriteIndex) * bytesPerSprite;
        const int top = static_cast<int>(oam[base]) - 16;
        if(static_cast<int>(ly.value()) < top || static_cast<int>(ly.value()) >= top + spriteHeight) continue;

        sprites[spriteTotal++] = {spriteIndex, oam[base + 1]};
    }

    // Smaller X coordinates have higher priority. Equal-X sprites are
    // resolved by lower OAM index. Draw low priority first so high priority can
    // overwrite it last.
    std::sort(sprites.begin(), sprites.begin() + static_cast<std::ptrdiff_t>(spriteTotal),
              [](const SpriteOnLine& left, const SpriteOnLine& right){
                  if(left.x != right.x) return left.x < right.x;
                  return left.index < right.index;
              });

    const std::size_t lineOffset = static_cast<std::size_t>(ly) * screenWidth;

    for(std::size_t sortedIndex = spriteTotal; sortedIndex > 0; --sortedIndex){
        const uint8_t spriteIndex = sprites[sortedIndex - 1].index;
        const std::size_t base = static_cast<std::size_t>(spriteIndex) * bytesPerSprite;

        const int top = static_cast<int>(oam[base]) - 16;
        const int left = static_cast<int>(oam[base + 1]) - 8;
        uint8_t tileId = oam[base + 2];
        const uint8_t attributes = oam[base + 3];

        const bool useOBP1 = (attributes & 0x10) != 0;
        const bool flipX = (attributes & 0x20) != 0;
        const bool flipY = (attributes & 0x40) != 0;
        const bool behindBackground = (attributes & 0x80) != 0;

        int spriteY = static_cast<int>(ly) - top;
        if(flipY) spriteY = spriteHeight - 1 - spriteY;

        if(spriteHeight == 16){
            tileId = static_cast<uint8_t>(tileId & 0xFE);
            if(spriteY >= 8){
                ++tileId;
                spriteY -= 8;
            }
        }

        for(int spriteX = 0; spriteX < 8; ++spriteX){
            const int screenX = left + spriteX;
            if(screenX < 0 || screenX >= static_cast<int>(screenWidth)) continue;

            const uint8_t tileX = static_cast<uint8_t>(flipX ? 7 - spriteX : spriteX);
            const uint8_t colorId = tilePixel(tileId, tileX, static_cast<uint8_t>(spriteY), true);

            // OBJ color 0 is transparent regardless of the selected object palette.
            if(colorId == 0) continue;

            const std::size_t x = static_cast<std::size_t>(screenX);
            if(behindBackground && backgroundColors[x] != 0) continue;

            frame[lineOffset + x] = paletteShade(useOBP1 ? obp1.value() : obp0.value(), colorId);
        }
    }
}

uint8_t PPU::tilePixel(uint8_t tileId, uint8_t pixelX, uint8_t pixelY, bool useUnsignedTileIds) const{
    uint16_t tileOffset = 0;

    if(useUnsignedTileIds) tileOffset = static_cast<uint16_t>(tileId) * bytesPerTile;
    else{
        // Signed tile IDs are centered on 0x9000. Relative to VRAM's 0x8000
        // base this is offset 0x1000, then -128..127 tiles around it.
        const int16_t signedId = static_cast<int8_t>(tileId);
        tileOffset = static_cast<uint16_t>(signedTileBaseOffset + signedId * bytesPerTile);
    }

    const uint16_t rowOffset = static_cast<uint16_t>(pixelY * 2);
    const uint8_t lowByte = vram[tileOffset + rowOffset];
    const uint8_t highByte = vram[tileOffset + rowOffset + 1];
    const uint8_t bit = static_cast<uint8_t>(7 - pixelX);

    const uint8_t lowBit = static_cast<uint8_t>((lowByte >> bit) & 0x01);
    const uint8_t highBit = static_cast<uint8_t>((highByte >> bit) & 0x01);
    return static_cast<uint8_t>((highBit << 1) | lowBit);
}

uint8_t PPU::paletteShade(uint8_t palette, uint8_t colorId){
    const uint8_t shift = static_cast<uint8_t>(colorId * 2);
    return static_cast<uint8_t>((palette >> shift) & 0x03);
}