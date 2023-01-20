#pragma once
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <chrono>

inline const uint64_t HashString(const std::string& str) {
    uint64_t value = 0x792fb382bafe973cUL;
    const char* cstr = str.c_str();
    for (int i = 0; i < str.length(); i++) {
        value <<= (cstr[i] & 0b00000111);
        value ^= cstr[i];
        value ^= value > 2;
    }
    return value;
}

