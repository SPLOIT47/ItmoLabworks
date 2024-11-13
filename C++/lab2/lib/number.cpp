#include "number.h"
#include <cstring>

bool TakeBit(const int2023_t& value, uint16_t bit_position) {
  return (value.bytes[bit_position / 8] >> (bit_position % 8)) & 1;
}

void SetBit(int2023_t& value, uint16_t bit_position) {
    value.bytes[bit_position / 8] |= (1 << (bit_position % 8));
}

bool IsNegative(const int2023_t& value) {
  return (value.bytes[kSizeBytes - 1] & (1 << 6));
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i < kSizeBytes; ++i) {
    if (lhs.bytes[i] != rhs.bytes[i]) {
      return false;
    }
  }
  return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs == rhs);
}

int2023_t operator++(int2023_t& value) {
    value = value + from_int(1);
    return value;
}

int2023_t operator+=(int2023_t& lhs, const int2023_t& rhs) {
  lhs = lhs + rhs;
  return lhs;
}

int2023_t operator*=(int2023_t& lhs, const int2023_t& rhs) {
  lhs = lhs * rhs;
  return lhs;
}

int2023_t operator-(const int2023_t& value) {
  int2023_t result = value;
  for (int i = 0; i < kSizeBytes; ++i) {
    result.bytes[i] = ~result.bytes[i];
  }
  ++result;
  return result;
}

int2023_t abs(const int2023_t& value) {
  return IsNegative(value) ? -value : value;
} 

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = kSizeBytes - 1; i >= 0; --i) {
    if (lhs.bytes[i] < rhs.bytes[i]) {
      return true;
    } 
    else if (lhs.bytes[i] > rhs.bytes[i]) {
      return false;
    } 
  }
  return false;
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs < rhs);
}

bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
  return rhs >= lhs;
}

int2023_t operator<<(const int2023_t& number, int shift) {
  int2023_t result = number;
  int bytes_shifts = shift / 8;
  int bits_shifts = shift % 8;

  for (int i = kSizeBytes - 1; i >= 0; i--) {
    if (i >= bytes_shifts) {
      result.bytes[i] = number.bytes[i - bytes_shifts];
    } else {
      result.bytes[i] = 0;
    }
    if (i > bytes_shifts) {
      result.bytes[i] <<= bits_shifts;
      result.bytes[i] |= (number.bytes[i - bytes_shifts - 1] >> (8 - bits_shifts));
    } else {
        result.bytes[i] <<= bits_shifts;
    }
  }
  return result;
}


int2023_t operator>>(const int2023_t& lhs, const int shift) {
  if (shift >= kSizeBits) {
    return from_int(0);
  }

  int2023_t result = lhs;
  int bytes_shifts = shift / 8;
  int bits_shifts = shift % 8;

  for (int i = 0; i < kSizeBytes; ++i) {
    if (i + bytes_shifts < kSizeBytes) {
      result.bytes[i] = lhs.bytes[i + bytes_shifts];
    } else {
      result.bytes[i] = 0;
    }
    if (i + bytes_shifts + 1 < kSizeBytes) {
      result.bytes[i] >>= bits_shifts;
      result.bytes[i] |= (lhs.bytes[i + bytes_shifts + 1] << (8 - bits_shifts));
    } else {
      result.bytes[i] >>= bits_shifts;
    }
  }
  return result;
}

int2023_t from_int(int32_t i) {

  int2023_t result;
  bool is_result_negative = (i < 0);
  i = abs(i);

  for (int j = 0; j < 31; j++) {
    if (i & (1 << j)) {
      result.bytes[j/8] |= (1 << (j % 8));
    }
  }
  return is_result_negative ? -result : result;
}

int2023_t from_string(const char* buff) {

  bool is_result_negative = (*buff == '-');
  const int2023_t kBase10 = from_int(10);
  const int kBuffLen = strlen(buff);
  int2023_t result;

  for (int i = (is_result_negative ? 1 : 0); i < kBuffLen; ++i) {
    if (!isdigit(buff[i])) {
      std::cerr << "Invalid character in input string\n";
      exit(EXIT_FAILURE);
    }
    int digit = buff[i] - '0';
    result *= kBase10;
    result += from_int(digit);
  }
  return is_result_negative ? -result : result;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {

  int2023_t result;
  uint8_t carry = 0;

  for (int i = 0; i < kSizeBytes; ++i) {
    uint32_t tmp = lhs.bytes[i] + rhs.bytes[i] + carry;
    result.bytes[i] = tmp;
    carry = tmp >> 8;
  }
  return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  return lhs + -rhs;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {

  int2023_t result = from_int(0);
  bool if_result_negative = IsNegative(lhs) != IsNegative(rhs); 
  int2023_t abs_lhs = abs(lhs);
  int2023_t abs_rhs = abs(rhs);
  int2023_t zero = from_int(0);

  if (abs_lhs == zero || abs_rhs == zero) {
    return zero;
  }
  for (int i = 0; i < kSizeBits; ++i) {
    if(TakeBit(abs_lhs, i)){
        int2023_t tmp = abs_rhs << i;
        result += tmp;
    }
  }
  return if_result_negative ? -result : result;
}

int2023_t operator/(const int2023_t &lhs, const int2023_t &rhs) {

  if(rhs == from_int(0)) {
    std::cerr << "Division by zero attempted\n";
    exit(EXIT_FAILURE);
  }

  int2023_t result = from_int(0);
  int2023_t tmp;
  bool is_result_negative = IsNegative(lhs) != IsNegative(rhs);
  int2023_t abs_lhs = abs(lhs);
  int2023_t abs_rhs = abs(rhs);
  
  if (abs_lhs < abs_rhs) {
    return result;
  }
  for(int i = kSizeBits - 1; i >= 0; --i) {  
    tmp = tmp << 1;
    tmp.bytes[0] = tmp.bytes[0] & 0xFE | TakeBit(abs_lhs, i);
    if(!(tmp < abs_rhs)) {
      tmp = tmp - abs_rhs;
      SetBit(result, i);
    }
  }
  return is_result_negative && result != from_int(0) ? -result : result;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  for (int i = kSizeBits - 1; i >= 0; --i) {
    bool bit = TakeBit(value, i);
    stream << bit;
  }
  return stream;
}
