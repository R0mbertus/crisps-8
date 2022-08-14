#include "chip_8.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <random>

const SHORT START = 0x200;
const std::array<BYTE, 0x50> fontset = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8() {
    for (int i = 0; i < (int)fontset.size(); i++) {
        memory[0x50 + i] = fontset[i];
    }
    pc = START, I = 0, sp = 0, opcode = 0,
    delayTimer = 0, soundTimer = 0;
    srand(time(0));
}

std::array<WORD, (64*32)> Chip8::getDisplay() {
    return display;
}

void Chip8::loadRom(const char* filePath) {
    std::ifstream fs;
    fs.open(filePath, std::ios::binary);
    fs.seekg(0, fs.end);
    int size = fs.tellg();
    fs.seekg(0, fs.beg);
    char* buffer = new char[size];
    fs.read(buffer, size);
    
    for (int i = 0; i < size; i++) {
        memory[START + i] = buffer[i];
    }
}

SHORT Chip8::nibble(SHORT val, SHORT val_to_binary_and, int bits)
{
    return ((val & val_to_binary_and) >> bits);
}

void Chip8::instructions() {
    SHORT instruction = memory[pc]<<8 | memory[pc+1];
    BYTE nibblet = nibble(instruction, 0xF000, 12);
    printf("%lX %X\n", instruction, nibblet);
    pc += 2;
    switch (nibblet)
    {
    case 0x0: {
        switch (instruction)
        {
        case 0x00E0: {
            std::fill(display.begin(), display.end(), 0);
            }break;
        case 0x00EE: {
            pc = stack[sp--];
            }break;
        }
        }break;
    case 0x1: {
        pc = instruction & 0x0FFF;
        }break;
    case 0x2: {
        stack[++sp] = pc;
        pc = instruction & 0x0FFF;
        }break;
    case 0x3: {
        nibblet = instruction & 0x00FF;
        SHORT x = nibble(instruction, 0x0F00, 8);
        if (vRegisters[x] == nibblet) {pc += 2;}
        }break;
    
    case 0x4: {
        nibblet = instruction & 0x00FF;
        SHORT x = nibble(instruction, 0x0F00, 8);
        if (vRegisters[x] != nibblet) {pc += 2;}
        }break;
    case 0x5: {
        SHORT x = nibble(instruction, 0x0F00, 8);
        SHORT y = nibble(instruction, 0x00F0, 4);
        if (vRegisters[x] == vRegisters[y]) {pc += 2;}
        }break;
    case 0x6: {
        nibblet = instruction & 0x00FF;
        SHORT x = nibble(instruction, 0x0F00, 8);
        vRegisters[x] = nibblet;
        }break;
    case 0x7: {
        nibblet = instruction & 0x00FF;
        SHORT x = nibble(instruction, 0x0F00, 8);
        vRegisters[x] += nibblet;
        }break;
    case 0x8: {
        nibblet = instruction & 0x000F;
        SHORT x = nibble(instruction, 0x0F00, 8);
        SHORT y = nibble(instruction, 0x00F0, 4);
        switch (nibblet)
        {
        case 0x0: {
            vRegisters[x] = vRegisters[y];
            }break;
        case 0x1: {
            vRegisters[x] |= vRegisters[y];
            }break;
        case 0x2: {
            vRegisters[x] &= vRegisters[y];
            }break;
        case 0x3: {
            vRegisters[x] ^= vRegisters[y];
            }break;
        case 0x4: {
            vRegisters[0xF] = (vRegisters[x] + vRegisters[y]) > 255;
            vRegisters[x] += vRegisters[y];
            }break;
        case 0x5: {
            vRegisters[0xF] = vRegisters[x] > vRegisters[y];
            vRegisters[x] -= vRegisters[y];
            }break;
        case 0x6: {
            vRegisters[0xF] = vRegisters[x] & 1;
            vRegisters[x] /= 2;
            }break;
        case 0x7: {
            vRegisters[0xF] = vRegisters[y] > vRegisters[x];
            vRegisters[x] = vRegisters[y] - vRegisters[x];
            }break;
        case 0xE: {
            vRegisters[0xF] = (vRegisters[x] >> 7) & 1;
            vRegisters[x] *= 2;
            }break;
        }break;
        }
    case 0x9: {
        SHORT x = nibble(instruction, 0x0F00, 8);
        SHORT y = nibble(instruction, 0x00F0, 4);
        if (vRegisters[x] == vRegisters[y]) pc *= 2;
        }break;
    case 0xA: {
        I = instruction & 0x0FFF;
        }break;
    case 0xB: {
        pc = (instruction & 0x0FFF) + vRegisters[0];
        }break;
    case 0xC: {
        SHORT x = nibble(instruction, 0x0F00, 8);
        vRegisters[x] = (instruction & 0x00FF) & (rand()%256);
        }break;
    case 0xD: {
        //Display
        }break;
    case 0xE: { //keys
        nibblet = instruction & 0x00FF;
        switch (nibblet)
        {
        case 0x9E: {
            }break;
        case 0xA1: {
            }break;
        }
        }break;
    case 0xF: {
        nibblet = instruction & 0x00FF;
        SHORT x = nibble(instruction, 0x0F00, 8);
        switch (nibblet)
        {
        case 0x07: {
            vRegisters[x] = delayTimer;
            }break;
        case 0x0A: {
            //keypress
            }break;
        case 0x15: {
            delayTimer = vRegisters[x];
            }break;
        case 0x18: {
            soundTimer = vRegisters[x];
            }break;
        case 0x1E: {
            I += vRegisters[x];
            }break;
        case 0x29: {
            I = (vRegisters[x] *  5) + 0x50;
            }break;
        case 0x33: {
            }break;
        case 0x55: {
            for (int i = 0; i < 16; i++) {
                memory[I + i] = vRegisters[i];
            }
            }break;
        case 0x65: {
            for (int i = 0; i < 16; i++) {
                vRegisters[i] = memory[I + i];
            }
            }break;
        }
        }break;
    }
}
