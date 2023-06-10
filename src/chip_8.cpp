#include "chip_8.hh"

#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>

#include "chip_8_definitions.hh"

Chip8::Chip8() {
    for (size_t i = 0; i < defaults::fontset_size; i++) {
        memory_[defaults::fontset_size + i] = defaults::fontset[i];
    }
}

std::array<WORD, defaults::screen_size> Chip8::getDisplay() {
    return display_;
}

void Chip8::loadRom(const char *file_path) {
    FILE *fp = std::fopen(file_path, "r");
    std::fseek(fp, 0, SEEK_END);
    const size_t rom_size = std::ftell(fp);
    std::fseek(fp, 0, SEEK_SET);

    if (rom_size < defaults::memory_size - defaults::rom_start) {
        std::fread(&memory_[defaults::rom_start], sizeof(BYTE), rom_size, fp);
        fclose(fp);
    }
    else {
        std::cerr << "Rom size too big\n";
        std::exit(1);
    }
}

SHORT Chip8::take_chunk(SHORT val, SHORT val_to_binary_and, int bits) {
    return ((val & val_to_binary_and) >> bits);
}

void Chip8::setKey(BYTE key, BYTE state) {
    keypad_[key] = state;
    if (state != 0) {
        latest_key_ = key;
    }
    else {
        latest_key_ = 0xFF;
    }
}

void Chip8::executeInstruction() {
    opcode_ = memory_[pc_] << 8 | memory_[pc_ + 1];
    const SHORT kk = opcode_ & 0x00FF;
    const SHORT x = take_chunk(opcode_, 0x0F00, 8);
    const SHORT y = take_chunk(opcode_, 0x00F0, 4);

    pc_ += 2;
    const BYTE left_byte = take_chunk(opcode_, 0xF000, 12);
    switch (left_byte) {
        case 0x0: {
            switch (opcode_) {
                case 0x00E0: {
                    std::fill(display_.begin(), display_.end(), 0);
                    draw_ = true;
                    break;
                }
                case 0x00EE: {
                    pc_ = stack_[sp_];
                    sp_--;
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
            stack_[++sp_] = pc_;
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
            switch (opcode_ & 0xF) {
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
                    const SHORT sum = v_registers_[x] + v_registers_[y];
                    v_registers_[0xF] = static_cast<BYTE>(sum > 0xFF);
                    v_registers_[x] = sum & 0xFF;
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
            pc_ = ((opcode_ & 0x0FFF) + v_registers_[0]) & 0x0FFF;
            break;
        }
        case 0xC: {
            v_registers_[x] = kk & distrib_(gen_);
            break;
        }
        case 0xD: {
            v_registers_[0xF] = 0;
            draw_ = true;

            const BYTE height = opcode_ & 0x000F;
            const BYTE x_pos = v_registers_[x] % defaults::screen_height;
            const BYTE y_pos = v_registers_[y] % defaults::screen_width;
            
            for (WORD i = 0; i < height; i++) {
                BYTE data = memory_[I_ + i];
                const WORD row = (v_registers_[y] + i) % 32;

                for(WORD j = 0; j < 8; j++) {
                    const WORD pixel = (data & 0x80U) >> 7;
                    const WORD column = (v_registers_[x] + j) % 64;
                    const WORD offset = row * 64 + column;

                    if (pixel == 1) {
                        if (display_[offset] != 0) {
                            display_[offset] = 0;
                            v_registers_[0xF] = 1;
                        }
                        else {
                            display_[offset] = 0xFFFFFF;
                        }
                    }

                    data <<= 1;
                }
            }
            break;
        }
        case 0xE: {
            switch (kk) {
                case 0x9E: {
                    if (v_registers_[x] == latest_key_) pc_ += 2;
                    break;
                }
                case 0xA1: {
                    if (v_registers_[x] != latest_key_) pc_ += 2;
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
                    if (keypad_[latest_key_] != 0) v_registers_[x] = latest_key_;
                    else pc_ -= 2;
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
                    I_ = (I_ + v_registers_[x]) & 0xFFF;
                    break;
                }
                case 0x29: {
                    I_ = (v_registers_[x] * 5) & 0xFFF;
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

    if (delay_timer_ != 0) {
        --delay_timer_;
    }
    if (sound_timer_ != 0) {
        --sound_timer_;
    }
}


