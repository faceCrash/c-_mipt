#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int binary_log_int(int val) {
  if (val <= 0) {
    throw std::invalid_argument("Value must be positive");
  }

  unsigned int u_val = static_cast<unsigned int>(val);
  int log2_val = 0;

  while (u_val > 1) {
    u_val >>= 1;
    ++log2_val;
  }

  return log2_val;
}

union FloatInt {
  float f;
  unsigned int i;
};

int binary_log_float(float val) {
  if (val <= 0.0f) {
    throw std::invalid_argument("Value must be positive");
  }

  FloatInt fi;
  fi.f = val;
  unsigned int bits = fi.i;

  unsigned int exponent = (bits >> 23) & 0xFF;
  unsigned int mantissa = bits & 0x7FFFFF;

  if (exponent == 0xFF) {
    throw std::invalid_argument("Infinity or NaN");
  } else if (exponent == 0) {
    int highest_bit = 0;

    while (mantissa > 1) {
      mantissa >>= 1;
      highest_bit++;
    }

    return -149 + highest_bit;
  } else {
    // No need to put this inside else
    return static_cast<int>(exponent) - 127;
  }
  
  // Function does not have a default return value, bad c++ practice
}

int main() {
  assert(binary_log_int(1) == 0);
  assert(binary_log_int(2) == 1);
  assert(binary_log_int(3) == 1);
  assert(binary_log_int(4) == 2);
  assert(binary_log_int(1024) == 10);
  assert(binary_log_int(2147483647) == 30); // Max 32-bit signed int

  std::cout << "Integer binary locarithm tests passed!\n";

  assert(binary_log_float(1.0f) == 0);
  assert(binary_log_float(2.0f) == 1);
  assert(binary_log_float(3.0f) == 1);
  assert(binary_log_float(4.0f) == 2);
  assert(binary_log_float(1024.0f) == 10);
  assert(binary_log_float(0.5f) == -1);
  assert(binary_log_float(0.25f) == -2);

  assert(binary_log_float(1.5f) == 0);
  assert(binary_log_float(3.5f) == 1);
  assert(binary_log_float(15.9f) == 3);

  float denorm1 = std::pow(2.0f, -127.0f);
  assert(binary_log_float(denorm1) == -127);

  float denorm2 = std::pow(2.0f, -149.0f);
  assert(binary_log_float(denorm2) == -149);

  std::cout << "Float binary locarithm tests passed!\n";


  return 0;
}

// Score is 10/10 
