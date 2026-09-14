#ifndef CLOCK_H
#define CLOCK_H
// CPU timings in M-Cycles, 1 M Cycle = 4 T Cycles

#include <cstdint>

class Clock{
public:
    Clock();
    void addTime(const uint8_t timestep);

    uint8_t readDIV() const;
    uint8_t readTIMA() const;
    uint8_t readTMA() const;
    uint8_t readTAC() const;

    void writeDIV();
    void writeTIMA(uint8_t val);
    void writeTMA(uint8_t val);
    void writeTAC(uint8_t val);

    bool consumeTimeInterrupt();

private:
    uint16_t dividerCounter = 0;

    uint8_t tima = 0;
    uint8_t tma = 0;
    uint8_t tac = 0;

    bool reloadPending = false;
    uint8_t reloadDelay = 0;
    bool timerInterruptRequested = false;

    void tickTCycle();
    void incrementTIMA();
    bool timerSignal() const;
    uint8_t selectedDividerBit() const;
    void handleFallingEdge(bool oldSig, bool newSig);
};

#endif