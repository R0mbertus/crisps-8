#include <SDL2/SDL.h>
#include <iostream>
#include "sdl2_init.h"
#include "chip_8.h"

int main(int argc, char* argv[]) {

    SDL_Window* window = NULL;

    if (init_window(window) > 0) {
        std::cout << "Couldn't create windows, exiting...\n";
        return 1;
    }

    Chip8 chip8;

    
    // The window is open: could enter program loop here (see SDL_PollEvent())
    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}