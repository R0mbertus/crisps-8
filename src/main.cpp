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

    const char * temp = argv[3];
    chip8.loadRom(temp);
    while (true) {
        chip8.instructions();
        display.update(chip8.getDisplay());
        if (display.EventHandler()) break;
    }

    return 0;
}