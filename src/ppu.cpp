#include "ppu.h"
#include <algorithm>

void PPU:tick(uint8_t mCYcles){
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

uint8_t PPU::readLCDC() const{
    return lcdc;
}

uint8_t PPU::readSTAT() const{
    const uint16_t coincidence = (ly == lyc) ? 0x04 : 0x00;
    const uint16_t modeBits = static_cast<uint8_t>(mode);
    return static_cast<uint8_t>(0x80 | statInterruptEnable | coincidence | modeBits);
}

uint8_t PPU::readLY() const{
    return ly;
}

uint8_t PPU::readLYC() const{
    return lyc;
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

void PPU::writeLYC(uint8_t val){
    lyc = val;
    updateSTATInterruptLine();
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
    return (lcdc & 0x80) != 0;
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