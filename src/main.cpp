#include "display.h"
#include "chip_8.h"
#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "<Scale> <Delay> <ROM>\n"; 
        return 1;
    }
    
    Display display(std::stoi(argv[1]));
    
    Chip8 chip8;
    
    display.update(chip8.getDisplay());

    const char *rom = argv[3];
    chip8.loadRom(rom);
    
    uint32_t startTick;
    uint32_t frameTick;
    uint32_t delayPerFrame = std::stoi(argv[2]);
    
    while (true) {
        startTick = SDL_GetTicks();
        
        chip8.instructions();
        display.update(chip8.getDisplay());
        if (display.EventHandler()) break;
        chip8.setKeypad(display.getKeypad());

        frameTick = SDL_GetTicks() - startTick;
        if (frameTick < delayPerFrame) {
            SDL_Delay(delayPerFrame - frameTick);
        }
    }

    return 0;
}