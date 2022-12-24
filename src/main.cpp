#include "display.h"
#include "chip_8.h"
#include "SDL.h"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n"; 
        return 1;
    }
    
    Display display(std::stoi(argv[1]));
    
    Chip8 chip8;
    
    display.update(chip8.getDisplay());

    const char *rom = argv[3];
    chip8.loadRom(rom);

    uint32_t start_tick;
    uint32_t frame_tick;
    uint32_t delay_per_frame = std::stoi(argv[2]);
    auto last_time = std::chrono::high_resolution_clock::now();

    while (true) {
        if (display.EventHandler(chip8)) break;

        auto current_time = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_time).count();

        if (dt > delay_per_frame) {
            last_time = current_time;
            
            chip8.instructions();
            if (chip8.draw_) {
                display.update(chip8.getDisplay());
                chip8.draw_ = false;
            }
        }
    }

    return 0;
}