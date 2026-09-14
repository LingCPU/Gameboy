#pragma once

#include <cstdint>

enum class Button{
    Up,
    Down,
    Left,
    Right,
    A,
    B,
    Select,
    Start
};

class Input{
public:
    bool buttonPressed(Button button);
    bool buttonReleased(Button button);
    
    bool write(uint8_t val);
    uint8_t read() const;

private:
    bool setButton(Button button, bool pressed);
    bool hasFallingEdge(uint8_t oldVal) const;

    // Active low: 
    // 1 = released
    // 0 = pressed
    uint8_t directions = 0x0F;
    uint8_t buttons = 0x0F;

    // FF00 bits 4-5.
    // 1 = group not selected
    // 0 = group selected
    uint8_t select = 0x30;
};