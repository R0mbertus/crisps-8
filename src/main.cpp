#include <SDL2/SDL.h>
#include <iostream>
#include "display.h"
#include "chip_8.h"

int main() {

    SDL_Window* window = NULL;
    if (init_window(window) > 0) {
        std::cout << "Couldn't create window, exiting...\n";
        return 1;
    }

    Chip8 chip8;
    const char * temp = "data/PONG";
    chip8.loadRom(temp);
    //chip8.printMemory();
    chip8.instructions();
    
    // The window is open: could enter program loop here (see SDL_PollEvent())
    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}