#pragma once
#include <cstdint>
#include <string>
#include <cstdio>
#include <chrono>

#define VERSION_CODE(major, minor, rev) (((major) << 24) | ((minor) << 16) | (rev))
#define VERSION_MAJOR(ver) (((ver) >> 24) & 0xff)
#define VERSION_MINOR(ver) (((ver) >> 16) & 0xff)
#define VERSION_REV(ver) ((ver) & 0xffff)
#define FORMAT_VERSION(buffer, bufferLen, version) snprintf((buffer), (bufferLen), "Version %i.%02i.%04i", VERSION_MAJOR(version), VERSION_MINOR(version), VERSION_REV(version))

#define INVALID_HASH ((Hash)(0ull))
typedef int64_t Hash;

inline const Hash RandomHash() {
    static Hash value = 0x792fb382bafe973cL;
    value <<= (value & 0b00000111);
    value ^= (value >> 32);
    value ^= (value >> 2);
    value ^= ((42069 + (value % 0xfff)) << 32);
    return value;
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
