#include "input.h"

bool Input::buttonPressed(Button button){
    return setButton(button, true);
}

bool Input::buttonReleased(Button button){
    return setButton(button, false);
}

bool Input::write(uint8_t value){
    const uint8_t oldValue = read();
    select = value & 0x30;
    return hasFallingEdge(oldValue);
}

uint8_t Input::read() const{
    uint8_t low = 0x0F;

    // Bit 4 = 0 selects the directional buttons.
    if((select & 0x10) == 0) low &= directions;

    // Bit 5 = 0 selects A/B/Select/Start.
    if ((select & 0x20) == 0) low &= buttons;

    // Bits 6-7 read high.
    return static_cast<uint8_t>(0xC0 | select | low);
}

bool Input::setButton(Button button, bool pressed){
    const uint8_t oldValue = read();

    uint8_t* group = nullptr;
    uint8_t bit = 0;

    switch(button){
        case Button::Right:
            group = &directions;
            bit = 0;
            break;

        case Button::Left:
            group = &directions;
            bit = 1;
            break;

        case Button::Up:
            group = &directions;
            bit = 2;
            break;

        case Button::Down:
            group = &directions;
            bit = 3;
            break;

        case Button::A:
            group = &buttons;
            bit = 0;
            break;

        case Button::B:
            group = &buttons;
            bit = 1;
            break;

        case Button::Select:
            group = &buttons;
            bit = 2;
            break;

        case Button::Start:
            group = &buttons;
            bit = 3;
            break;
    }

    const uint8_t mask = static_cast<uint8_t>(1u << bit);

    if(pressed) *group &= static_cast<uint8_t>(~mask); // Active-low: pressing clears the bit.
    else *group |= mask; // Releasing sets the bit.
    
    return hasFallingEdge(oldValue);
}

bool Input::hasFallingEdge(uint8_t oldValue) const{
    const uint8_t newValue = read();
    const uint8_t falling = static_cast<uint8_t>((oldValue & static_cast<uint8_t>(~newValue)) & 0x0F);

    return falling != 0;
}