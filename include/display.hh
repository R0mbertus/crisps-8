#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>

#include <SDL2/SDL.h>

#include "chip_8_definitions.hh"
#include "chip_8.hh"

constexpr std::array<SDL_Keycode, 16> keymap_{
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

class Display {
private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;

    int scale_;
    int pitch_ = sizeof(WORD) * defaults::screen_height;
    std::array<BYTE, defaults::small_size> keypad_{};

public:
    explicit Display(int set_scale = defaults::scale);
    ~Display();
    void update(std::array<WORD, (defaults::screen_height * defaults::screen_width)> display);
    bool EventHandler(Chip8 &chip8);
    void keypress(SDL_Event event, BYTE val_to_set, Chip8 &chip8);
};

#endif
