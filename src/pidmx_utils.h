#pragma once
#include <stdint.h>
#include <string>
#include <stdio.h>
#include <chrono>

#define VERSION(major, minor, rev) (((major) << 24) | ((minor) << 16) | (rev))
#define VERSION_MAJOR(ver) (((ver) >> 24) & 0xff)
#define VERSION_MINOR(ver) (((ver) >> 16) & 0xff)
#define VERSION_REV(ver) ((ver) & 0xffff)

#define INVALID_HASH 0
typedef int64_t Hash;
inline const Hash HashString(const std::string& str) {
    Hash value = 0x792fb382bafe973cL;
    const char* cstr = str.c_str();
    for (int i = 0; i < str.length(); i++) {
        value <<= (cstr[i] & 0b00000111);
        value ^= cstr[i];
        value ^= value > 2;
    }
    return value | 1;
}

inline bool VerifyTextNumeric(char* str) {
	if (str[0] == 0) {
		return false;
	}
	while (*str) {
		if (*str < '0' || *str > '9') {
			return false;
		}
		str++;
	}
	return true;
}

bool FixturePatchStringToUniverseChannel(char *text, size_t len, int *universe, int *channel);
