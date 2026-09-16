#include "serial.h"
#include <cstdio>

Serial::Serial() = default;

uint8_t Serial::readData() const{
    return data.value();
}

uint8_t Serial::readCtrl() const{
    // On the DMG only bits 7 and 0 are meaningful
    return static_cast<uint8_t>(ctrl.value() | 0x7E);
}

void Serial::writeData(uint8_t byte){
    data.set(byte);
}

void Serial::writeCtrl(uint8_t byte){
    // DMG serial uses bit 7 to request/start a transfer and bit 0 to select the
    // clock source. Bit 1 is CGB-only and the remaining bits are unused for
    ctrl.set(static_cast<uint8_t>(byte & 0x81));

    const bool transferRequested = (ctrl.value() & 0x80) != 0;
    const bool internalClock = (ctrl.value() & 0x01) != 0;

    if(!transferRequested){
        internalTransferActive = false;
        transferMCycleCounter = 0;
        return;
    }

    if(internalClock) beginTransfer();
    else{
        internalTransferActive = false;
        transferMCycleCounter = 0;
    }
}

void Serial::tick(uint8_t mCycles){
    if(!internalTransferActive) return;
    transferMCycleCounter = static_cast<uint16_t>(transferMCycleCounter + mCycles);
    if(transferMCycleCounter >= internalTransferMCycles) completeTransfer();
}

bool Serial::consumeInterrupt(){
    const bool requested = interruptRequested;
    interruptRequested = false;
    return requested;
}

void Serial::beginTransfer(){
    internalTransferActive = true;
    transferMCycleCounter = 0;
    transmittedByte = data.value();
}

void Serial::completeTransfer(){
    internalTransferActive = false;
    transferMCycleCounter = 0;

    data.set(0xFF);
    ctrl.set(static_cast<uint8_t>(ctrl.value() & 0x01));
    interruptRequested = true;

    std::putchar(static_cast<int>(transmittedByte));
    std::fflush(stdout);
}