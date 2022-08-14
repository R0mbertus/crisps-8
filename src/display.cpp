#include "display.h"
#include "chip_8_definitions.h"

Display::Display(int scale) {
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(
        "chips-8",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        32 * scale, 64 * scale,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {   //In case the window couldn't be created, throw exception
        throw "Could not create window: ";
    }

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );
    if (renderer == NULL) {
        throw "Could not create renderer: ";
    }

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        32, 64
    );
    if (texture == NULL) {
        throw "Could not create texture: ";
    }
}

Display::~Display() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Display::update(std::array<WORD, (64*32)> display) {
    int pitch = sizeof(WORD) * 32;
    SDL_UpdateTexture(texture, NULL, &display, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
