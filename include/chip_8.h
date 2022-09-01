#ifndef CHIP_8_H
#define CHIP_8_H
#include "chip_8_definitions.h"
#include <array>
#include <vector>

constexpr const int kHeight = 64;
constexpr const int kWidth = 32;
constexpr const int kDisplaySize = kHeight * kWidth;
constexpr const int kMemorySize = 0x1000;
constexpr const int kSmallSize = 0x10;

class Chip8 {
    std::array<WORD, kDisplaySize> display_{};
    //std::array<std::array<BYTE, 4>, 4> keypad_{};
    std::array<BYTE, kMemorySize> memory_{};
    std::array<BYTE, kSmallSize> v_registers_{};
    std::array<SHORT, kSmallSize> stack_{};
    SHORT opcode_;
    SHORT pc_;
    SHORT I_;
    BYTE sp_;
    BYTE delay_timer_;
    BYTE sound_timer_;

public:
    Chip8();
    std::array<WORD, (kDisplaySize)> getDisplay();
    void loadRom(const char *filePath);
    static SHORT nibble(SHORT val, SHORT val_to_binary_and, int bits);
    void instructions();
};

#endif
