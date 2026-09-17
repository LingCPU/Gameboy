#include <SDL3/SDL.h>

#include <cstdint>
#include <cstdio>
#include <exception>
#include <optional>
#include <string>
#include <utility>

#include "cartridge.h"
#include "gameboy.h"
#include "ppu.h"

namespace{
    constexpr int pixelScale = 2;
    constexpr int windowWidth = static_cast<int>(PPU::screenWidth) * pixelScale;
    constexpr int windowHeight = static_cast<int>(PPU::screenHeight) * pixelScale;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* screenTexture = nullptr;

    Gameboy* gameboy = nullptr;
    bool shouldExit = false;

    std::optional<Button> getButton(SDL_Keycode key){
        switch(key){
            case SDLK_UP:        return Button::Up;
            case SDLK_DOWN:      return Button::Down;
            case SDLK_LEFT:      return Button::Left;
            case SDLK_RIGHT:     return Button::Right;
            case SDLK_X:         return Button::A;
            case SDLK_Z:         return Button::B;
            case SDLK_BACKSPACE: return Button::Select;
            case SDLK_RETURN:    return Button::Start;
            default:             return std::nullopt;
        }
    }

    uint32_t shadeToARGB(uint8_t shade){
        switch(shade & 0x03){
            case 0: return 0xFFFFFFFF;
            case 1: return 0xFFAAAAAA;
            case 2: return 0xFF555555;
            case 3: return 0xFF000000;
        }
        return 0xFF000000;
    }

    void processEvents(){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_KEY_DOWN:
                    if(event.key.repeat) break;
                    if(event.key.key == SDLK_ESCAPE){
                        shouldExit = true;
                        break;
                    }
                    if(const auto button = getButton(event.key.key); button){
                        gameboy->buttonPressed(*button);
                    }
                    break;

                case SDL_EVENT_KEY_UP:
                    if(event.key.repeat) break;
                    if(const auto button = getButton(event.key.key); button){
                        gameboy->buttonReleased(*button);
                    }
                    break;

                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                case SDL_EVENT_QUIT:
                    shouldExit = true;
                    break;

                default:
                    break;
            }
        }
    }

    void drawFrame(const PPU::FrameBuffer& frame){
        processEvents();

        void* pixelMemory = nullptr;
        int pitch = 0;
        if(!SDL_LockTexture(screenTexture, nullptr, &pixelMemory, &pitch)){
            std::fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
            shouldExit = true;
            return;
        }

        auto* bytes = static_cast<uint8_t*>(pixelMemory);
        for(std::size_t y = 0; y < PPU::screenHeight; ++y){
            auto* row = reinterpret_cast<uint32_t*>(bytes + y * static_cast<std::size_t>(pitch));
            for(std::size_t x = 0; x < PPU::screenWidth; ++x){
                const std::size_t index = y * PPU::screenWidth + x;
                row[x] = shadeToARGB(frame[index]);
            }
        }

        SDL_UnlockTexture(screenTexture);

        if(!SDL_RenderClear(renderer)){
            std::fprintf(stderr, "SDL_RenderClear failed: %s\n", SDL_GetError());
            shouldExit = true;
            return;
        }

        if(!SDL_RenderTexture(renderer, screenTexture, nullptr, nullptr)){
            std::fprintf(stderr, "SDL_RenderTexture failed: %s\n", SDL_GetError());
            shouldExit = true;
            return;
        }

        SDL_RenderPresent(renderer);
    }

    bool isClosed(){
        return shouldExit;
    }

    void destroySDL(){
        if(screenTexture != nullptr) SDL_DestroyTexture(screenTexture);
        if(renderer != nullptr) SDL_DestroyRenderer(renderer);
        if(window != nullptr) SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::fprintf(stderr, "Usage: emulator <rom.gb>\n");
        return 1;
    }

    if(!SDL_Init(SDL_INIT_VIDEO)){
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if(!SDL_CreateWindowAndRenderer(
        "Gameboy Emulator",
        windowWidth,
        windowHeight,
        0,
        &window,
        &renderer)){
        std::fprintf(stderr, "SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
        destroySDL();
        return 1;
    }

    if(!SDL_SetRenderVSync(renderer, 1)){
        std::fprintf(stderr, "Warning: SDL_SetRenderVSync failed: %s\n", SDL_GetError());
    }

    screenTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        static_cast<int>(PPU::screenWidth),
        static_cast<int>(PPU::screenHeight)
    );

    if(screenTexture == nullptr){
        std::fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        destroySDL();
        return 1;
    }

    if(!SDL_SetTextureScaleMode(screenTexture, SDL_SCALEMODE_NEAREST)){
        std::fprintf(stderr, "Warning: SDL_SetTextureScaleMode failed: %s\n", SDL_GetError());
    }

    try{
        Cartridge cartridge{std::string{argv[1]}};
        Gameboy emulator(std::move(cartridge));
        gameboy = &emulator;
        emulator.run(isClosed, drawFrame);
        gameboy = nullptr;
    } catch(const std::exception& error){
        std::fprintf(stderr, "Emulator error: %s\n", error.what());
        destroySDL();
        return 1;
    }

    destroySDL();
    return 0;
}
