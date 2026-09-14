#include "clock.h"

Clock::Clock() = default;

void Clock::addTime(uint8_t mCycles){
    const uint16_t tCycles = static_cast<uint16_t>(mCycles) * 4;
    for(uint16_t i = 0; i < tCycles; ++i){
        tickTCycle();
    }
}

uint8_t Clock::readDIV() const{
    return static_cast<uint8_t>(dividerCounter >> 8);
}

uint8_t Clock::readTIMA() const{
    return tima;
}

uint8_t Clock::readTMA() const{
    return tma;
}

uint8_t Clock::readTAC() const{
    return static_cast<uint8_t>(tac | 0xF8); // Only bits 0-2 are implemented by TAC. On DMG hardware the unused upper bits read back as 1.
}

void Clock::writeDIV(){
    const bool oldSig = timerSignal();
    dividerCounter = 0;
    const bool newSig = timerSignal();
    handleFallingEdge(oldSig, newSig);
}

void Clock::writeTIMA(uint8_t val){
    tima = val;
    reloadPending = false;
    reloadDelay = 0;
}

void Clock::writeTMA(uint8_t val){
    tma = val;
}

void Clock::writeTAC(uint8_t val){
    const bool oldSig = timerSignal();
    tac = static_cast<uint8_t>(val & 0x07);
    const bool newSig = timerSignal();
    handleFallingEdge(oldSig, newSig);
}

bool Clock::consumeTimerInterrupt(){
    const bool requested = timerInterruptRequested;
    timerInterruptRequested = false;
    return requested;
}

void Clock::tickTCycle(){
    // When TIMA overflows, real hardware waits four T-cycles before loading
    // TMA and requesting the timer interrupt.
    if(reloadPending && reloadDelay > 0){
        --reloadDelay;
        if(reloadDelay == 0){
            tima = tma;
            reloadPending = false;
            timerInterruptRequest = true;
        }
    }
    const bool oldSig = timerSignal();
    ++dividerCounter;
    const bool newSig = timerSignal();
    handleFallingEdge(oldSig, newSig);
}

void Clock::incrementTIMA(){
    // During the four-T-cycle overflow window, TIMA is waiting for TMA to be
    // reloaded and should not begin another increment sequence.
    if(reloadPending) return;
    if(tima == 0xFF){
        tima = 0x00;
        reloadPending = true;
        reloadDelay = 4;
        return;
    }
    ++tima;
}

bool Clock::timerSignal() const{
    const bool enabled = (tac & 0x04) != 0;
    if(!enabled) return false;
    const uint8_t bit = selectedDividerBit();
    return ((dividerCounter >> bit) & 0x01) != 0;
}

uint8_t Clock::selectedDividerBit() const{
    // TAC bits 1-0 choose which internal divider bit drives TIMA:
    // 00 -> bit 9 (4096 Hz)
    // 01 -> bit 3 (262144 Hz)
    // 10 -> bit 5 (65536 Hz)
    // 11 -> bit 7 (16384 Hz)
    switch(tac & 0x03){
        case 0x00: return 9;
        case 0x01: return 3;
        case 0x02: return 5;
        case 0x03: return 7;
    }
    return 9;
}

void Clock::handleFallingEdge(bool oldSignal, bool newSignal){
    if(oldSignal && !newSignal) incrementTIMA();
}
