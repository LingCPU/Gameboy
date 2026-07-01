#pragma once

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
    void buttonPressed(Button button);
    void buttonReleased(Button button);

private:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool a = false;
    bool b = false;
    bool select = false;
    bool start = false;
};