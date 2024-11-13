#pragma once
#include <cinttypes>
#include <iostream>

const int kSizeBytes = 253;
const int kSizeBits = 2023;
struct int2023_t {
  unsigned char bytes[kSizeBytes] = {0};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");
int2023_t operator++(int2023_t& value);

int2023_t operator+=(int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*=(int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& value);

int2023_t abs(const int2023_t& value);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

bool operator<=(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator<<(const int2023_t& number, int shift);

int2023_t operator>>(const int2023_t& lhs, const int shift);

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
