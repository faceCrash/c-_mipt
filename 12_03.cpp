#include <iostream>
#include <vector>
#include <string>
#include <string_view>

/**
 * Task 12.03: Longest Palindromic Substring using DP.
 * 
 * Requirements:
 * - No brute force.
 * - Use caching (Dynamic Programming).
 * - Use linearized std::vector<bool> of size N x N for caching.
 * - Use std::string_view for optimization.
 */

std::string_view find_longest_palindrome(std::string_view s) {
    size_t n = s.length();
    if (n <= 1) return s;

    // Linearized table of size N x N. 
    // table[i * n + j] is true if s[i...j] is a palindrome.
    std::vector<bool> table(n * n, false);

    size_t start = 0;
    size_t max_len = 1;

    // All substrings of length 1 are palindromes
    for (size_t i = 0; i < n; ++i) {
        table[i * n + i] = true;
    }

    // Check for substrings of length 2
    for (size_t i = 0; i < n - 1; ++i) {
        if (s[i] == s[i + 1]) {
            table[i * n + (i + 1)] = true;
            start = i;
            max_len = 2;
        }
    }

    // Check for substrings of length 3 or more
    // k is the length of the substring
    for (size_t k = 3; k <= n; ++k) {
        // i is the starting index
        for (size_t i = 0; i < n - k + 1; ++i) {
            // j is the ending index
            size_t j = i + k - 1;

            // Check if inner substring is a palindrome and outer characters match
            if (table[(i + 1) * n + (j - 1)] && s[i] == s[j]) {
                table[i * n + j] = true;
                if (k > max_len) {
                    start = i;
                    max_len = k;
                }
            }
        }
    }

    return s.substr(start, max_len);
}

int main() {
    std::string input;
    std::cout << "Enter a string: ";
    if (!std::getline(std::cin, input)) return 0;

    std::string_view longest = find_longest_palindrome(input);
    std::cout << "Longest palindromic substring: " << longest << std::endl;

    return 0;
}
