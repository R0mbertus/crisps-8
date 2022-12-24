#ifndef DISPLAY_H
#define DISPLAY_H
#include "SDL.h"
#include <array>
#include <stdexcept>
#include <iostream>
#include "chip_8_definitions.h"
#include "chip_8.h"

constexpr std::array<SDL_Keycode, 16> keymap_{
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

class Display
{
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    int scale_;
    int pitch_ = sizeof(WORD) * kHeight;
    std::array<BYTE, kSmallSize> keypad_{};

public:
    explicit Display(int set_scale = kSmallSize);
    ~Display();
    void update(std::array<WORD, (kHeight * kWidth)> display);
    bool EventHandler(Chip8 &chip8);
    void keypress(SDL_Event event, BYTE val_to_set, Chip8 &chip8);
};

#endif
