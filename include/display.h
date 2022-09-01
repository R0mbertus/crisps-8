#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <array>
#include "chip_8_definitions.h"

class Display
{
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
public:
    explicit Display(int scale = 8);
    ~Display();
    void update(std::array<WORD, (64*32)> display);
};

#endif
