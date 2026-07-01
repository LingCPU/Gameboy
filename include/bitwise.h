#pragma once
#include <cstdint>

inline uint16_t composeByte(const uint8_t high, const uint8_t low){
    return static_cast<uint16_t>(high) << 8 | low;
}

inline bool checkBit(const uint8_t byte, const int bit){
    return (byte & (1 << bit)) != 0;
}

inline uint8_t setBit(const uint8_t byte, const int bit){
    return byte | (1 << bit);
}