#ifndef CHIP_8_H
#define CHIP_8_H
#include <array>
#include <vector>
#include "chip_8_definitions.h"

class Chip8
{
private:
    std::array<BYTE, 0x1000> memory;
    std::array<BYTE, 0x10>   vRegisters;
    std::array<SHORT, 0x10>  stack;
    SHORT PC;           //Program counter
    SHORT I;            //Index Register
    BYTE  SP;
    BYTE  delayTimer;
    BYTE  soundTimer;

public:
    Chip8();
    ~Chip8();
};

#endif
