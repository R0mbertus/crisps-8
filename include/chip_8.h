#ifndef CHIP_8_H
#define CHIP_8_H

#include <array>
#include <cstdint>
#include <random>

#include "chip_8_definitions.h"

const std::array<BYTE, kFontsetSize> kFontset = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,// 0
    0x20, 0x60, 0x20, 0x20, 0x70,// 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,// 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,// 3
    0x90, 0x90, 0xF0, 0x10, 0x10,// 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,// 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,// 6
    0xF0, 0x10, 0x20, 0x40, 0x40,// 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,// 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,// 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,// A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,// B
    0xF0, 0x80, 0x80, 0x80, 0xF0,// C
    0xE0, 0x90, 0x90, 0x90, 0xE0,// D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,// E
    0xF0, 0x80, 0xF0, 0x80, 0x80 // F
};

class Chip8 {
private:
    std::array<WORD, kDisplaySize> display_{};
    std::array<BYTE, kSmallSize> keypad_{};
    std::array<BYTE, kMemorySize> memory_{};
    std::array<BYTE, kSmallSize> v_registers_{};
    std::array<SHORT, kSmallSize> stack_{};
    SHORT opcode_{};
    SHORT pc_ = kRomStart;
    SHORT I_{};
    BYTE sp_{};
    BYTE latest_key_{};
    BYTE delay_timer_{};
    BYTE sound_timer_{};

    std::mt19937 gen_{std::random_device{}()};
    std::uniform_int_distribution<std::mt19937::result_type> distrib_{0, 255};

public:
    Chip8();
    ~Chip8() = default;
    std::array<WORD, (kDisplaySize)> getDisplay();
    static SHORT take_chunk(SHORT val, SHORT val_to_binary_and, int bits);
    void loadRom(const char *file_path);
    void setKey(BYTE key, BYTE state);
    void executeInstruction();

    bool draw_{true};
};

#endif
