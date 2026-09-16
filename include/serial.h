#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>
#include "register.h"

class Serial{
public:
    Serial();

    uint8_t readData() const;
    uint8_t readCtrl() const;

    void writeData(uint8_t byte);
    void writeCtrl(uint8_t byte);

    void tick(uint8_t mCycles);
    bool consumeInterrupt();

private:
    // DMG internal serial clock is 8192 Hz. Since the CPU runs at
    // 1,048,576 M-cycles/second, one bit takes 128 M-cycles and a full
    // 8-bit transfer takes 1024 M-cycles.
    static constexpr uint16_t internalTransferMCycles = 1024;

    Register data;
    Register ctrl;

    uint16_t transferMCycleCounter = 0;
    uint8_t transmittedByte = 0;

    bool internalTransferActive = false;
    bool interruptRequested = false;

    void beginTransfer();
    void completeTransfer();
};

#endif