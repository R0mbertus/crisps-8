#ifndef CHIP_8_H
#define CHIP_8_H

#include <array>
#include <cstdint>
#include <random>

#include "chip_8_definitions.hh"

class Chip8 {
private:
    std::array<WORD, defaults::screen_size> display_{};
    std::array<BYTE, defaults::small_size> keypad_{};
    std::array<BYTE, defaults::memory_size> memory_{};
    std::array<BYTE, defaults::small_size> v_registers_{};
    std::array<SHORT, defaults::small_size> stack_{};
    SHORT opcode_{};
    SHORT pc_ = defaults::rom_start;
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
    std::array<WORD, (defaults::screen_size)> getDisplay();
    static SHORT take_chunk(SHORT val, SHORT val_to_binary_and, int bits);
    void loadRom(const char *file_path);
    void setKey(BYTE key, BYTE state);
    void executeInstruction();

    bool draw_{true};
};

#endif
