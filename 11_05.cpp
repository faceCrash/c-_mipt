#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>
#include <iterator>

/**
 * @brief Item 11.05 (11.26): Demonstration of Ranges, Algorithms and Views.
 */

// --- 1. Custom Fibonacci View ---

class Fibonacci : public std::ranges::view_interface<Fibonacci> {
private:
    class Iterator {
    public:
        using value_type = unsigned long long;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;

        Iterator() : current_(0), next_(1) {}
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

        bool operator==(const Iterator& other) const = default;

    private:
        unsigned long long current_;
        unsigned long long next_;
    };

public:
    Fibonacci() = default;
    Iterator begin() const { return Iterator(0, 1); }
    std::unreachable_sentinel_t end() const { return std::unreachable_sentinel; }
};

// --- 2. transform_if Algorithm ---
// Implemented based on ranges::copy_if and ranges::transform as requested.

template <std::ranges::input_range R, typename Pred, typename Trans>
auto transform_if(R&& r, Pred pred, Trans trans) {
    using V = std::ranges::range_value_t<R>;
    std::vector<V> filtered;
    // Step 1: Copy elements matching the predicate
    std::ranges::copy_if(r, std::back_inserter(filtered), pred);
    
    using U = std::invoke_result_t<Trans, V>;
    std::vector<U> result;
    // Step 2: Transform the filtered elements
    std::ranges::transform(filtered, std::back_inserter(result), trans);
    
    return result;
}

// --- 3. MAE & MSE Calculations ---
// Implemented based on standard numerical algorithms (std::transform_reduce).

template <std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mae(R1&& r1, R2&& r2) {
    auto n = std::ranges::distance(r1);
    if (n == 0) return 0.0;
    
    return std::transform_reduce(
        std::ranges::begin(r1), std::ranges::end(r1),
        std::ranges::begin(r2), 0.0,
        std::plus<>(),
        [](auto a, auto b) { return std::abs(a - b); }
    ) / n;
}

template <std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mse(R1&& r1, R2&& r2) {
    auto n = std::ranges::distance(r1);
    if (n == 0) return 0.0;
    
    return std::transform_reduce(
        std::ranges::begin(r1), std::ranges::end(r1),
        std::ranges::begin(r2), 0.0,
        std::plus<>(),
        [](auto a, auto b) { return (a - b) * (a - b); }
    ) / n;
}

int main() {
    std::cout << "--- Task 11.05: Ranges, Algorithms and Views ---\n\n";

    // --- 4. Ranges Algorithms Demo ---
    std::vector<int> v = {1, 2, 2, 3, 4, 5};
    std::cout << "1. Ranges Algorithms Demo:\n";
    std::cout << "   Original: "; for (int x : v) std::cout << x << " "; std::cout << "\n";

    std::ranges::replace(v, 2, 10);
    std::cout << "   Replace(2, 10): "; for (int x : v) std::cout << x << " "; std::cout << "\n";

    std::ranges::fill(v, 7);
    std::cout << "   Fill(7): "; for (int x : v) std::cout << x << " "; std::cout << "\n";

    v = {1, 1, 2, 3, 3, 3, 4};
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    std::cout << "   Unique: "; for (int x : v) std::cout << x << " "; std::cout << "\n";

    std::ranges::rotate(v, v.begin() + 1);
    std::cout << "   Rotate (left 1): "; for (int x : v) std::cout << x << " "; std::cout << "\n";

    std::vector<int> sampled;
    std::ranges::sample(v, std::back_inserter(sampled), 2, std::mt19937{std::random_device{}()});
    std::cout << "   Sample (2): "; for (int x : sampled) std::cout << x << " "; std::cout << "\n\n";

    // --- 5. transform_if Demo ---
    std::vector<int> nums = {1, 2, 3, 4, 5, 6};
    auto even_squares = transform_if(nums, [](int n) { return n % 2 == 0; }, [](int n) { return n * n; });
    std::cout << "2. transform_if (squares of even): ";
    for (int x : even_squares) std::cout << x << " ";
    std::cout << "\n\n";

    // --- 6. MAE & MSE Demo ---
    std::vector<double> y_true = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> y_pred = {1.1, 1.9, 3.2, 3.8};
    std::cout << "3. MAE: " << calculate_mae(y_true, y_pred) << "\n";
    std::cout << "   MSE: " << calculate_mse(y_true, y_pred) << "\n\n";

    // --- 7. Views Demo ---
    std::cout << "4. Views Demo:\n";
    
    // views::filter and views::drop
    auto filter_drop = nums | std::views::filter([](int n) { return n > 2; }) | std::views::drop(2);
    std::cout << "   Filter (>2) -> Drop(2): "; for (int x : filter_drop) std::cout << x << " "; std::cout << "\n";

    // views::join
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
    auto joined = nested | std::views::join;
    std::cout << "   Join nested: "; for (int x : joined) std::cout << x << " "; std::cout << "\n";

    // views::zip
    auto zipped = std::views::zip(nums, std::views::reverse(nums));
    std::cout << "   Zip (nums, reversed): "; 
    for (auto [a, b] : zipped) std::cout << "(" << a << "," << b << ") "; std::cout << "\n";

    // views::stride workaround (since C++23 stride is not available in many compilers yet)
    auto strided = nums | std::views::filter([i = 0](int) mutable { return i++ % 2 == 0; });
    std::cout << "   Stride (step 2): "; for (int x : strided) std::cout << x << " "; std::cout << "\n\n";

    // --- 8. Fibonacci View Demo ---
    std::cout << "5. Fibonacci View Demo:\n";
    Fibonacci fib;
    std::cout << "   First 10 Fibonacci numbers: ";
    for (auto n : fib | std::views::take(10)) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
