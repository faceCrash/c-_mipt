#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
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

std::vector<std::string> make_strings(size_t size_1, size_t size_2) {
  auto engine = std::mt19937{std::random_device{}()};
  auto dist = std::uniform_int_distribution<int>{'a', 'z'};
  auto result = std::vector<std::string>{};
  result.reserve(size_1);
  for (size_t i = 0; i < size_1; ++i) {
    auto string = std::string(size_2, ' ');
    for (size_t j = 0; j < size_2; ++j) {
      string[j] = static_cast<char>(dist(engine));
    }
    result.push_back(string);
  }
  return result;
}

int main() {
  std::vector<HashInfo> funcs = {
      {"RS", RSHash},   {"JS", JSHash},     {"PJW", PJWHash},
      {"ELF", ELFHash}, {"BKDR", BKDRHash}, {"SDBM", SDBMHash},
      {"DJB", DJBHash}, {"DEK", DEKHash},   {"AP", APHash}};

  std::vector<int> sizes = {1000, 5000, 10000, 20000, 50000, 75000, 100000};
  std::ofstream csv("collision_results.csv");

  csv << "Size";
  for (const auto &info : funcs) {
    csv << "," << info.name;
  }
  csv << "\n";

  for (int n : sizes) {
    std::cout << "Processing size: " << n << "..." << std::endl;
    auto dataset = make_strings(n, 10);
    csv << n;

    for (const auto &info : funcs) {
      std::unordered_set<uint32_t> seen;
      int collisions = 0;
      for (const auto &s : dataset) {
        uint32_t h = info.func(s.c_str(), (uint32_t)s.length());
        if (!seen.insert(h).second) {
          collisions++;
        }
      }
      csv << "," << collisions;
    }
    csv << "\n";
  }

  csv.close();
  std::cout << "\nResults exported to collision_results.csv" << std::endl;

  return 0;
}
