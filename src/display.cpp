#include "display.h"
#include "chip_8_definitions.h"

Display::Display(int setScale) {
    SDL_Init(SDL_INIT_VIDEO);
    scale = setScale;

    window_ = SDL_CreateWindow(
            "chips-8",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            64 * scale, 32 * scale,
            SDL_WINDOW_SHOWN);
    if (window_ == nullptr) {//In case the window couldn't be created, throw exception
        throw std::runtime_error("Could not create window: ");
    }

    renderer_ = SDL_CreateRenderer(
            window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr) {
        throw std::runtime_error("Could not create renderer: ");
    }

    texture_ = SDL_CreateTexture(
            renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
            64, 32);
    if (texture_ == nullptr) {
        throw std::runtime_error("Could not create texture: ");
    }
}

Display::~Display() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Display::update(std::array<WORD, (64 * 32)> display) {
    SDL_UpdateTexture(texture_, nullptr, &display, pitch);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

bool Display::EventHandler() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    return true;
            }
        default:
            return false;
    }
    return false;
}
