#include "display.h"

Display::Display(int set_scale) {
    SDL_Init(SDL_INIT_VIDEO);
    scale_ = set_scale;

    window_ = SDL_CreateWindow(
            "chips-8",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            kHeight * scale_, kWidth * scale_,
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
            renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
            kHeight, kWidth);
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

void Display::update(std::array<WORD, (kHeight * kWidth)> display) {
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
    for (BYTE i = 0; i < kSmallSize; i++) {
        if (event.key.keysym.sym == keymap_[i]) {
            chip8.setKey(i, val_to_set);
        }
    }
}
