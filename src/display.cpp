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
            renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
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
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        return true;
                    default:
                        break;
                }
            case SDL_KEYDOWN:
                keypress(event, 1);
                break;
            case SDL_KEYUP:
                keypress(event, 0);
                break;
            default:
                break;
        }
    }
    return false;
}

void Display::keypress(SDL_Event event, BYTE valToSet) {
    switch (event.key.keysym.sym) {
        case SDLK_x:
            keypad_[0] = valToSet;
        case SDLK_1:
            keypad_[1] = valToSet;
        case SDLK_2:
            keypad_[2] = valToSet;
        case SDLK_3:
            keypad_[3] = valToSet;
        case SDLK_q:
            keypad_[4] = valToSet;
        case SDLK_w:
            keypad_[5] = valToSet;
        case SDLK_e:
            keypad_[6] = valToSet;
        case SDLK_r:
            keypad_[7] = valToSet;
        case SDLK_a:
            keypad_[8] = valToSet;
        case SDLK_s:
            keypad_[9] = valToSet;
        case SDLK_d:
            keypad_[10] = valToSet;
        case SDLK_f:
            keypad_[11] = valToSet;
        case SDLK_z:
            keypad_[12] = valToSet;
        case SDLK_4:
            keypad_[13] = valToSet;
        case SDLK_c:
            keypad_[14] = valToSet;
        case SDLK_v:
            keypad_[15] = valToSet;
    }
}

std::array<BYTE, 16> Display::getKeypad() {
    return keypad_;
}
