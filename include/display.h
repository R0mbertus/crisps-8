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

    int scale;
    int pitch = sizeof(WORD) * 64;
    std::array<BYTE, 16> keypad_{};
public:
    explicit Display(int setScale = 16);
    ~Display();
    void update(std::array<WORD, (64*32)> display);
    bool EventHandler();
    void keypress(SDL_Event event, BYTE valToSet);
    std::array<BYTE, 16> getKeypad();
};



#endif
