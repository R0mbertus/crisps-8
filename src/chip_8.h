#ifndef CHIP_8_H
#define CHIP_8_H
#include <array>
#include <vector>
#include "chip_8_definitions.h"

class Chip8
{
private:
    std::array<std::array<WORD, 64>, 32> display{};
    std::array<std::array<BYTE, 4>, 4>   keypad{};
    std::array<BYTE, 0x1000> memory{};
    std::array<BYTE, 0x10>   vRegisters{};
    std::array<SHORT, 0x10>  stack{};
    SHORT opcode;
    SHORT pc;
    SHORT I;
    BYTE  sp;
    BYTE  delayTimer;
    BYTE  soundTimer;

public:
    Chip8(void);
    void loadRom(const char* filePath);
    void printMemory(void);
    SHORT  nibble(SHORT val, SHORT val_to_binary_and, int bits);
    void instructions(void);
};

#endif
