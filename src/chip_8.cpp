#include "chip_8.h"
#include "chip_8_definitions.h"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>

const int kStart = 0x200;
const int kFontsetSize = 0x50;
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

Chip8::Chip8() {
    for (int i = 0; i < static_cast<int>(kFontset.size()); i++) {
        memory_[kFontsetSize + i] = kFontset[i];
    }
    pc_ = kStart, I_ = 0, sp_ = 0, opcode_ = 0,
    delay_timer_ = 0, sound_timer_ = 0;
    srand(time(nullptr));
}

std::array<WORD, kDisplaySize> Chip8::getDisplay() {
    return display_;
}

void Chip8::loadRom(const char *filePath) {
    std::ifstream fs;
    fs.open(filePath, std::ios::binary);
    fs.seekg(0, std::ifstream::end);
    int size = fs.tellg();
    fs.seekg(0, std::ifstream::beg);
    char *buffer = new char[size];
    fs.read(buffer, size);

    for (int i = 0; i < size; i++) {
        memory_[kStart + i] = buffer[i];
    }
}

SHORT Chip8::nibble(SHORT val, SHORT val_to_binary_and, int bits) {
    return ((val & val_to_binary_and) >> bits);
}

auto Chip8::instructions() -> void {
    opcode_ = memory_[pc_] << 8 | memory_[pc_ + 1];
    BYTE nibblet = nibble(opcode_, 0xF000, 12);
    pc_ += 2;
    switch (nibblet) {
        case 0x0: {
            switch (opcode_) {
                case 0x00E0: {
                    std::fill(display_.begin(), display_.end(), 0);
                } break;
                case 0x00EE: {
                    pc_ = stack_[sp_--];
                } break;
            }
        } break;
        case 0x1: {
            pc_ = opcode_ & 0x0FFF;
        } break;
        case 0x2: {
            stack_[++sp_] = pc_;
            pc_ = opcode_ & 0x0FFF;
        } break;
        case 0x3: {
            nibblet = opcode_ & 0x00FF;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            if (v_registers_[x] == nibblet) { pc_ += 2; }
        } break;
        case 0x4: {
            nibblet = opcode_ & 0x00FF;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            if (v_registers_[x] != nibblet) { pc_ += 2; }
        } break;
        case 0x5: {
            SHORT x = nibble(opcode_, 0x0F00, 8);
            SHORT y = nibble(opcode_, 0x00F0, 4);
            if (v_registers_[x] == v_registers_[y]) { pc_ += 2; }
        } break;
        case 0x6: {
            nibblet = opcode_ & 0x00FF;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            v_registers_[x] = nibblet;
        } break;
        case 0x7: {
            nibblet = opcode_ & 0x00FF;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            v_registers_[x] += nibblet;
        } break;
        case 0x8: {
            nibblet = opcode_ & 0x000F;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            SHORT y = nibble(opcode_, 0x00F0, 4);
            switch (nibblet) {
                case 0x0: {
                    v_registers_[x] = v_registers_[y];
                } break;
                case 0x1: {
                    v_registers_[x] |= v_registers_[y];
                } break;
                case 0x2: {
                    v_registers_[x] &= v_registers_[y];
                } break;
                case 0x3: {
                    v_registers_[x] ^= v_registers_[y];
                } break;
                case 0x4: {
                    v_registers_[0xF] = static_cast<BYTE>((v_registers_[x] + v_registers_[y]) > 255);
                    v_registers_[x] += v_registers_[y];
                } break;
                case 0x5: {
                    v_registers_[0xF] = static_cast<BYTE>(v_registers_[x] > v_registers_[y]);
                    v_registers_[x] -= v_registers_[y];
                } break;
                case 0x6: {
                    v_registers_[0xF] = v_registers_[x] & 1;
                    v_registers_[x] /= 2;
                } break;
                case 0x7: {
                    v_registers_[0xF] = static_cast<BYTE>(v_registers_[y] > v_registers_[x]);
                    v_registers_[x] = v_registers_[y] - v_registers_[x];
                } break;
                case 0xE: {
                    v_registers_[0xF] = (v_registers_[x] >> 7) & 1;
                    v_registers_[x] *= 2;
                } break;
            }
            break;
        }
        case 0x9: {
            SHORT x = nibble(opcode_, 0x0F00, 8);
            SHORT y = nibble(opcode_, 0x00F0, 4);
            if (v_registers_[x] != v_registers_[y]) pc_ += 2;
        } break;
        case 0xA: {
            I_ = opcode_ & 0x0FFF;
        } break;
        case 0xB: {
            pc_ = (opcode_ & 0x0FFF) + v_registers_[0];
        } break;
        case 0xC: {
            SHORT x = nibble(opcode_, 0x0F00, 8);
            v_registers_[x] = (opcode_ & 0x00FF) & (rand() % 256);
        } break;
        case 0xD: {
            BYTE x = nibble(opcode_, 0x0F00, 8);
            BYTE y = nibble(opcode_, 0x00F0, 4);
            BYTE height = opcode_ & 0x000F;
            BYTE x_pos = v_registers_[x] % kHeight;
            BYTE y_pos = v_registers_[y] % kWidth;
            v_registers_[0xF] = 0;
            for (BYTE row = 0; row < height; row++) {
                WORD data = memory_[I_ + row];
                for(unsigned int column = 0; column < 8; column++) {
                    BYTE pixel = data & (0x80u >> column);
                    if (pixel) {
                        if (display_[(y_pos + row) * kHeight + (x_pos + column)]) {
                            v_registers_[0xF] = 1;
                        }
                        display_[(y_pos + row) * kHeight + (x_pos + column)] ^= 0xFFFFFFFF;
                    }
                }
            }
        } break;
        case 0xE: {//keys
            nibblet = opcode_ & 0x00FF;
            SHORT x = nibble(opcode_, 0x0F00, 8);
            BYTE key = v_registers_[x];
            switch (nibblet) {
                case 0x9E: {
                    if (keypad_[key]) {
                        pc_ += 2;
                    }
                } break;
                case 0xA1: {
                    if (!keypad_[key]) {
                        pc_ += 2;
                    }
                } break;
            }
        } break;
        case 0xF: {
            nibblet = opcode_ & 0x00FF;
            BYTE x = nibble(opcode_, 0x0F00, 8);
            switch (nibblet) {
                case 0x07: {
                    v_registers_[x] = delay_timer_;
                } break;
                case 0x0A: {
                    for (BYTE i = 0; i < 16; i++) {
                        if (keypad_[i]) {
                            v_registers_[x] = i;
                            break;
                        }
                    }
                } break;
                case 0x15: {
                    delay_timer_ = v_registers_[x];
                } break;
                case 0x18: {
                    sound_timer_ = v_registers_[x];
                } break;
                case 0x1E: {
                    I_ += v_registers_[x];
                } break;
                case 0x29: {
                    I_ = (v_registers_[x] * 5) + kFontsetSize;
                } break;
                case 0x33: {
                    BYTE v_register_value = v_registers_[x];
                    memory_[I_ + 2] = v_register_value % 10;
                    v_register_value /= 10;
                    memory_[I_ + 1] = v_register_value % 10;
                    v_register_value /= 10;
                    memory_[I_] = v_register_value % 10;
                } break;
                case 0x55: {
                    for (int i = 0; i < 16; i++) {
                        memory_[I_ + i] = v_registers_[i];
                    }
                } break;
                case 0x65: {
                    for (int i = 0; i < 16; i++) {
                        v_registers_[i] = memory_[I_ + i];
                    }
                } break;
            }
        } break;
    }
}
