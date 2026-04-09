#include <iostream>
#include <ranges>
#include <iterator>
#include <concepts>

/**
 * @brief Item 11.05: Fibonacci sequence representation using C++20 Ranges.
 * Uses std::ranges::view_interface and CRTP for the interface definition.
 */
class Fibonacci : public std::ranges::view_interface<Fibonacci> {
private:
    /**
     * @brief Nested private class representing a forward iterator for Fibonacci numbers.
     */
    class Iterator {
    public:
        using value_type = unsigned long long;
        using difference_type = std::ptrdiff_t;
        // Mark as forward iterator
        using iterator_concept = std::forward_iterator_tag;

        Iterator() : current_(0), next_(1) {}
        
        // Constructor for Specific state
        Iterator(unsigned long long current, unsigned long long next) 
            : current_(current), next_(next) {}

        unsigned long long operator*() const { return current_; }

        Iterator& operator++() {
            unsigned long long temp = current_ + next_;
            current_ = next_;
            next_ = temp;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        /**
         * @brief Equality operator.
         * For an infinite range, comparing with a sentinel or end is usually required.
         */
        bool operator==(const Iterator& other) const = default;

    private:
        unsigned long long current_;
        unsigned long long next_;
    };

public:
    Fibonacci() = default;

    /**
     * @brief Creates an iterator pointing to the start of the sequence (0, 1).
     */
    Iterator begin() const { return Iterator(0, 1); }

    /**
     * @brief Returns a sentinel representing an unreachable end (sequence is infinite).
     */
    std::unreachable_sentinel_t end() const { return std::unreachable_sentinel; }
};

int main() {
    std::cout << "--- Task 11.05 Fibonacci View ---" << std::endl;

    Fibonacci fib;

    std::cout << "First 15 Fibonacci numbers using custom view and views::take:\n";
    for (auto val : fib | std::views::take(15)) {
        std::cout << val << " ";
    }
    std::cout << "\n\n";

    std::cout << "Fibonacci numbers > 100, skip 2, take 5:\n";
    auto complex_query = fib 
        | std::views::filter([](auto n) { return n > 100; })
        | std::views::drop(2)
        | std::views::take(5);

    for (auto val : complex_query) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
