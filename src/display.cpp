#include "display.hh"

#include <array>
#include <stdexcept>
#include <iostream>

#include <SDL2/SDL.h>

#include "chip_8_definitions.hh"
#include "chip_8.hh"

Display::Display(int set_scale) {
    SDL_Init(SDL_INIT_VIDEO);
    scale_ = set_scale;

    window_ = SDL_CreateWindow(
            "chips-8",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            defaults::screen_height * scale_, defaults::screen_width * scale_,
            SDL_WINDOW_SHOWN);

    renderer_ = SDL_CreateRenderer(
            window_, -1, SDL_RENDERER_ACCELERATED);

    texture_ = SDL_CreateTexture(
            renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
            defaults::screen_height, defaults::screen_width);

    if (!window_ || !renderer_ || !texture_) {
        std::cerr << SDL_GetError() << "\n\n";
        SDL_Quit();
        std::exit(1);
    }
}

Display::~Display() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Display::update(std::array<WORD, (defaults::screen_height * defaults::screen_width)> display) {
    SDL_UpdateTexture(texture_, nullptr, &display, pitch_);
    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

bool Display::EventHandler(Chip8 &chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        return true;
                    default:
                        break;
                }
                break;
            case SDL_KEYDOWN:
                keypress(event, 1, chip8);
                break;
            case SDL_KEYUP:
                keypress(event, 0, chip8);
                break;
            default:
                break;
        }
    }
    return false;
}

void Display::keypress(SDL_Event event, BYTE val_to_set, Chip8 &chip8) {
    for (BYTE i = 0; i < defaults::small_size; i++) {
        if (event.key.keysym.sym == keymap_[i]) {
            chip8.setKey(i, val_to_set);
        }
    }
}
