#include <SDL2/SDL.h>
#include <iostream>
#include "display.h"
#include "chip_8.h"

int main() {
    Display display;
    
    Chip8 chip8;
    
    display.update(chip8.getDisplay());

    const char * temp = "data/PONG";
    chip8.loadRom(temp);
    chip8.instructions();
    
    SDL_Delay(5000);

    return 0;
}