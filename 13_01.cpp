#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <algorithm>

/**
 * Converts a collection of 8-bit unsigned integers to a hex string.
 * Uses std::stringstream with standard manipulators for formatting.
 */
std::string to_hex_string(const std::vector<std::uint8_t>& data) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::right;
    for (const auto& byte : data) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

/**
 * Helper to convert a single hex character to its integer value using character subtraction.
 */
std::uint8_t char_to_nibble(char c) {
    if (c >= '0' && c <= '9') return static_cast<std::uint8_t>(c - '0');
    if (c >= 'a' && c <= 'f') return static_cast<std::uint8_t>(10 + (c - 'a'));
    return 0;
}

/**
 * Converts a hex string to a collection of 8-bit unsigned integers.
 * Uses character subtraction and bitwise operations as requested.
 * Assumes the string has an even number of characters and contains only lowercase hex digits.
 */
std::vector<std::uint8_t> from_hex_string(const std::string& hex) {
    std::vector<std::uint8_t> result;
    result.reserve(hex.length() / 2);
    
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::uint8_t high = char_to_nibble(hex[i]);
        std::uint8_t low = char_to_nibble(hex[i + 1]);
        result.push_back(static_cast<std::uint8_t>((high << 4) | low));
    }
    
    return result;
}

int main() {
    // Example collection
    std::vector<std::uint8_t> original = {0xba, 0xad, 0xf0, 0x0d, 0x01, 0x02, 0xff};
    
    std::cout << "Original bytes: ";
    for (int b : original) std::cout << std::hex << std::showbase << b << " ";
    std::cout << std::dec << std::endl;

    // Convert to hex string
    std::string hex_str = to_hex_string(original);
    std::cout << "Hex string: " << hex_str << std::endl;

    // Convert back from hex string
    std::vector<std::uint8_t> restored = from_hex_string(hex_str);
    
    std::cout << "Restored bytes: ";
    for (int b : restored) std::cout << std::hex << std::showbase << b << " ";
    std::cout << std::endl;

    // Validation
    if (original == restored) {
        std::cout << "Success: Collections match!" << std::endl;
    } else {
        std::cout << "Error: Collections do not match!" << std::endl;
    }

    return 0;
}
