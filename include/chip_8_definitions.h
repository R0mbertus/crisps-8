#ifndef CHIP_8_DEFINITIONS_H
#define CHIP_8_DEFINITIONS_H
#include <cstdint>

using BYTE = uint8_t;
using SHORT = uint16_t;
using WORD = uint32_t;

constexpr const BYTE kHeight = 64;
constexpr const BYTE kWidth = 32;
constexpr const BYTE kSmallSize = 0x10;
constexpr const SHORT kStart = 0x200;
constexpr const SHORT kFontsetSize = 0x50;
constexpr const WORD kDisplaySize = kHeight * kWidth;
constexpr const WORD kMemorySize = 0x1000;

#endif
