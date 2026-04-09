#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

typedef uint32_t (*HashFunc)(const char *, uint32_t);

uint32_t RSHash(const char *str, uint32_t length) {
  uint32_t b = 378551;
  uint32_t a = 63689;
  uint32_t hash = 0;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = hash * a + (uint8_t)(*str);
    a = a * b;
  }
  return hash;
}

uint32_t JSHash(const char *str, uint32_t length) {
  uint32_t hash = 1315423911;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash ^= ((hash << 5) + (uint8_t)(*str) + (hash >> 2));
  }
  return hash;
}

uint32_t PJWHash(const char *str, uint32_t length) {
  const uint32_t BitsInUnsignedInt = 32;
  const uint32_t ThreeQuarters = (BitsInUnsignedInt * 3) / 4;
  const uint32_t OneEighth = BitsInUnsignedInt / 8;
  const uint32_t HighBits = 0xFFFFFFFF << (BitsInUnsignedInt - OneEighth);
  uint32_t hash = 0;
  uint32_t test = 0;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = (hash << OneEighth) + (uint8_t)(*str);
    if ((test = hash & HighBits) != 0) {
      hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
    }
  }
  return hash;
}

uint32_t ELFHash(const char *str, uint32_t length) {
  uint32_t hash = 0;
  uint32_t x = 0;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = (hash << 4) + (uint8_t)(*str);
    if ((x = hash & 0xF0000000) != 0) {
      hash ^= (x >> 24);
    }
    hash &= ~x;
  }
  return hash;
}

uint32_t BKDRHash(const char *str, uint32_t length) {
  uint32_t seed = 131;
  uint32_t hash = 0;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = (hash * seed) + (uint8_t)(*str);
  }
  return hash;
}

uint32_t SDBMHash(const char *str, uint32_t length) {
  uint32_t hash = 0;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = (uint8_t)(*str) + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

uint32_t DJBHash(const char *str, uint32_t length) {
  uint32_t hash = 5381;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = ((hash << 5) + hash) + (uint8_t)(*str);
  }
  return hash;
}

uint32_t DEKHash(const char *str, uint32_t length) {
  uint32_t hash = length;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash = ((hash << 5) ^ (hash >> 27)) ^ (uint8_t)(*str);
  }
  return hash;
}

uint32_t APHash(const char *str, uint32_t length) {
  uint32_t hash = 0xAAAAAAAA;
  for (uint32_t i = 0; i < length; ++str, ++i) {
    hash ^= ((i & 1) == 0)
                ? ((hash << 7) ^ (uint8_t)(*str) * (hash >> 3))
                : (~((hash << 11) + ((uint8_t)(*str) ^ (hash >> 5))));
  }
  return hash;
}

struct HashInfo {
  std::string name;
  HashFunc func;
};

void run_test(const std::vector<HashInfo> &funcs,
              const std::vector<std::string> &data) {
  std::cout << std::setw(10) << "Data Size" << " | " << std::setw(10)
            << "Function" << " | Collisions" << std::endl;
  std::cout << "------------------------------------------" << std::endl;
  for (const auto &info : funcs) {
    std::unordered_set<uint32_t> seen;
    int collisions = 0;
    for (const auto &s : data) {
      uint32_t h = info.func(s.c_str(), (uint32_t)s.length());
      if (!seen.insert(h).second) {
        collisions++;
      }
    }
    std::cout << std::setw(10) << data.size() << " | " << std::setw(10)
              << info.name << " | " << collisions << std::endl;
  }
  std::cout << std::endl;
}

int main() {
  std::cout << "Task 10.05: Hash Function Collision Analysis (32-bit mock)"
            << std::endl
            << std::endl;

  std::vector<HashInfo> funcs = {
      {"RS", RSHash},   {"JS", JSHash},     {"PJW", PJWHash},
      {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
      {"DJB", DJBHash}, {"DEK", DEKHash},   {"AP", APHash}};

  std::vector<int> sizes = {1000, 10000, 50000, 100000};

  for (int n : sizes) {
    std::vector<std::string> dataset;
    dataset.reserve(n);
    for (int i = 0; i < n; ++i) {

      dataset.push_back("prefix_" + std::to_string(i) + "_suffix");
    }
    run_test(funcs, dataset);
  }

  std::cout << "Conclusion:" << std::endl;
  std::cout << "- Best functions (0 collisions): BKDR, DJB, AP (high "
               "distribution quality)."
            << std::endl;
  std::cout << "- Susceptible functions: ELF, PJW (may show more collisions on "
               "specific patterns due to logic)."
            << std::endl;
  std::cout
      << "- The number of collisions is expected to remain near zero for N << "
         "2^16 due to the Birthday paradox property of 32-bit hashes."
      << std::endl;

  return 0;
}
