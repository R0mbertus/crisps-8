#include "display.h"
#include "chip_8.h"
#include <SDL2/SDL.h>

int main() {
    Display display;
    
    Chip8 chip8;
    
    display.update(chip8.getDisplay());

    const char * temp = "data/PONG";
    chip8.loadRom(temp);
    while (true) {
        std::cout << "test\n";
        chip8.instructions();
        display.update(chip8.getDisplay());
    }

    return 0;
}