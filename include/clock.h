#ifndef CLOCK_H
#define CLOCK_H
// Clock class to manage the timing of the Gameboy's components

#include <cstdint>

class Clock{
public:
    Clock();
    void addTime(const uint8_t timestep);
};



#endif