#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <array>
#include "chip_8_definitions.h"

class Display
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
public:
    Display(int scale = 8);
    ~Display(void);
    void update(std::array<WORD, (64*32)> display);
};

#endif
