#include "chip_8.h"
#include "chip_8_definitions.h"
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>

Chip8::Chip8() {
    for (size_t i = 0; i < kFontsetSize; i++) {
        memory_[kFontsetSize + i] = kFontset[i];
    }

    srand(time(nullptr));
}

std::array<WORD, kDisplaySize> Chip8::getDisplay() {
    return display_;
}

void Chip8::loadRom(const char *filePath) {
    std::ifstream fs;
    fs.open(filePath, std::ios::binary);
    const int size = fs.tellg();
    fs.seekg(0, std::ifstream::beg);
    char *buffer = new char[size];
    fs.read(buffer, size);

    for (int i = 0; i < size; i++) {
        memory_[kStart + i] = buffer[i];
    }

    delete[] buffer;
}

SHORT Chip8::nibble(SHORT val, SHORT val_to_binary_and, int bits) {
    return ((val & val_to_binary_and) >> bits);
}

void Chip8::setKey(BYTE key, BYTE state) {
    keypad_[key] = state;
}

void Chip8::instructions() {
    opcode_ = memory_[pc_] << 8 | memory_[pc_ + 1];
    const SHORT kk = opcode_ & 0x00FF;
    const SHORT x = nibble(opcode_, 0x0F00, 8);
    const SHORT y = nibble(opcode_, 0x00F0, 4);

    pc_ += 2;
    const BYTE left_byte = nibble(opcode_, 0xF000, 12);
    switch (left_byte) {
        case 0x0: {
            switch (opcode_) {
                case 0x00E0: {
                    std::fill(display_.begin(), display_.end(), 0);
                    draw_ = true;
                    break;
                }
                case 0x00EE: {
                    --sp_;
                    pc_ = stack_[sp_];
                    pc_ += 2;
                    break;
                }
            }
            break;
        }
        case 0x1: {
            pc_ = opcode_ & 0x0FFF;
            break;
        }
        case 0x2: {
            pc_ -= 2;
            stack_[sp_++] = pc_;
            pc_ = opcode_ & 0x0FFF;
            break;
        }
        case 0x3: {
            if (v_registers_[x] == kk) pc_ += 2; 
            break;
        }
        case 0x4: {
            if (v_registers_[x] != kk) pc_ += 2;
            break;
        }
        case 0x5: {
            if (v_registers_[x] == v_registers_[y]) pc_ += 2;
            break;
        }
        case 0x6: {
            v_registers_[x] = kk;
            break;
        }
        case 0x7: {
            v_registers_[x] += kk;
            break;
        }
        case 0x8: {
            switch (kk) {
                case 0x0: {
                    v_registers_[x] = v_registers_[y];
                    break;
                }
                case 0x1: {
                    v_registers_[x] |= v_registers_[y];
                    break;
                }
                case 0x2: {
                    v_registers_[x] &= v_registers_[y];
                    break;
                }
                case 0x3: {
                    v_registers_[x] ^= v_registers_[y];
                    break;
                }
                case 0x4: {
                    v_registers_[0xF] = static_cast<BYTE>((v_registers_[x] + v_registers_[y]) > 0xFF);
                    v_registers_[x] += v_registers_[y];
                    break;
                }
                case 0x5: {
                    v_registers_[0xF] = static_cast<BYTE>(v_registers_[x] > v_registers_[y]);
                    v_registers_[x] -= v_registers_[y];
                    break;
                }
                case 0x6: {
                    v_registers_[0xF] = v_registers_[x] & 0x1;
                    v_registers_[x] >>= 1;
                    break;
                }
                case 0x7: {
                    v_registers_[0xF] = static_cast<BYTE>(v_registers_[x] < v_registers_[y]);
                    v_registers_[x] = v_registers_[y] - v_registers_[x];
                    break;
                }
                case 0xE: {
                    v_registers_[0xF] = v_registers_[x] >> 7;
                    v_registers_[x] <<= 1;
                    break;
                }
            }
            break;
        }
        case 0x9: {
            if (v_registers_[x] != v_registers_[y]) pc_ += 2;
            break;
        }
        case 0xA: {
            I_ = opcode_ & 0x0FFF;
            break;
        }
        case 0xB: {
            pc_ = (opcode_ & 0x0FFF) + v_registers_[0];
            break;
        }
        case 0xC: {
            v_registers_[x] = kk & rand();
            break;
        }
        case 0xD: {
            v_registers_[0xF] = 0;
            draw_ = true;

            const BYTE height = opcode_ & 0x000F;
            const BYTE x_pos = v_registers_[x] % kHeight;
            const BYTE y_pos = v_registers_[y] % kWidth;
            
            for (BYTE row = 0; row < height; row++) {
                WORD const data = memory_[I_ + row];
                for(unsigned int column = 0; column < 8; column++) {
                    const BYTE pixel = data & (0x80U >> column);
                    if (pixel) {
                        v_registers_[0xF] |= static_cast<BYTE>(display_[(y_pos + row) * kHeight + (x_pos + column)] == 1);
                        display_[(y_pos + row) * kHeight + (x_pos + column)] ^= 0xFFFFFFFF;
                    }
                }
            }
            break;
        }
        case 0xE: {
            switch (kk) {
                case 0x9E: {
                    if (keypad_[v_registers_[x]]) pc_ += 2;
                    break;
                }
                case 0xA1: {
                    if (!keypad_[v_registers_[x]]) pc_ += 2;
                    break;
                }
            }
            break;
        }
        case 0xF: {
            switch (kk) {
                case 0x07: {
                    v_registers_[x] = delay_timer_;
                    break;
                }
                case 0x0A: {
                    int key_pressed = 0;
                    for (BYTE i = 0; i < 16; i++) {
                        if (keypad_[i]) {
                            v_registers_[x] = i;
                            key_pressed = 1;
                            break;
                        }
                    }
                    if (!key_pressed) {
                        pc_ -= 2;
                    }  
                    break;
                }
                case 0x15: {
                    delay_timer_ = v_registers_[x];
                    break;
                }
                case 0x18: {
                    sound_timer_ = v_registers_[x];
                    break;
                }
                case 0x1E: {
                    v_registers_[0xF] = static_cast<BYTE>((I_ + v_registers_[x]) > 0xFFF);
                    I_ += v_registers_[x];
                    break;
                }
                case 0x29: {
                    I_ = (v_registers_[x] * 5);
                    break;
                }
                case 0x33: {
                    BYTE v_register_value = v_registers_[x];
                    memory_[I_ + 2] = v_register_value % 10;
                    v_register_value /= 10;
                    memory_[I_ + 1] = v_register_value % 10;
                    v_register_value /= 10;
                    memory_[I_] = v_register_value % 10;
                    break;
                }
                case 0x55: {
                    for (int i = 0; i <= x; i++) {
                        memory_[I_ + i] = v_registers_[i];
                    }
                    break;
                }
                case 0x65: {
                    for (int i = 0; i <= x; i++) {
                        v_registers_[i] = memory_[I_ + i];
                    }
                    break;
                }
            }
            break;
        }
    }

    if (delay_timer_) {
        --delay_timer_;
    }
    if (sound_timer_) {
        --sound_timer_;
    }
}
