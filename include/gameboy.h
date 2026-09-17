#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "input.h"
#include <functional>

class Gameboy{
public: 
    using ShouldCloseCallback = std::function<bool()>;
    using FrameCallback = std::function<void(const PPU::FrameBuffer&)>;
    
    Gameboy(Cartridge cartridge);

    void run(const ShouldCloseCallback& shouldClose, const FrameCallback& drawFrame);
    void buttonPressed(Button button);
    void buttonReleased(Button button);

    const PPU::FrameBuffer& frameBuffer() const;
    bool consumeFrameReady();

private:
    Cartridge cartridge;
    MMU mmu;
    CPU cpu;

    void tick();
};

#endif