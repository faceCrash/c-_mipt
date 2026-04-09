#include <algorithm>
#include <iostream>
#include <vector>
#include <ranges>
#include <numeric>
#include <cmath>
#include <random>

/**
 * @brief Item 11.05 (11.26): Ranges, Views, and Numeric Algorithms.
 */

/**
 * @brief Composed algorithm transform_if.
 * Uses views::filter and views::transform to filter and then transform a range.
 */
template<std::ranges::input_range R, typename Pred, typename Trans>
auto transform_if(R&& r, Pred pred, Trans trans) {
    return r | std::views::filter(pred) | std::views::transform(trans);
}

/**
 * @brief Mean Absolute Error (MAE) calculation.
 * sum(|y_true - y_pred|) / n
 */
template<std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mae(R1&& r1, R2&& r2) {
    double sum = 0;
    size_t count = 0;
    for (auto [v1, v2] : std::views::zip(r1, r2)) {
        sum += std::abs(v1 - v2);
        count++;
    }
    return count == 0 ? 0 : sum / count;
}

/**
 * @brief Mean Squared Error (MSE) calculation.
 * sum((y_true - y_pred)^2) / n
 */
template<std::ranges::input_range R1, std::ranges::input_range R2>
double calculate_mse(R1&& r1, R2&& r2) {
    double sum = 0;
    size_t count = 0;
    for (auto [v1, v2] : std::views::zip(r1, r2)) {
        double diff = v1 - v2;
        sum += diff * diff;
        count++;
    }
    return count == 0 ? 0 : sum / count;
}

int main() {
    std::cout << "--- Task 11.05 Algorithms & Views ---" << std::endl;

    // 1. Demos for ranges::algorithms
    std::vector<int> v = {1, 2, 2, 3, 4, 5};
    std::cout << "Original vector: ";
    for (int x : v) std::cout << x << " "; std::cout << "\n";

    // ranges::replace
    std::ranges::replace(v, 2, 10);
    std::cout << "After ranges::replace(2, 10): ";
    for (int x : v) std::cout << x << " "; std::cout << "\n";

    // ranges::fill
    std::ranges::fill(v, 7);
    std::cout << "After ranges::fill(7): ";
    for (int x : v) std::cout << x << " "; std::cout << "\n";

    // ranges::unique
    v = {1, 1, 2, 3, 3, 3, 4};
    auto [first, last] = std::ranges::unique(v);
    v.erase(first, last);
    std::cout << "After ranges::unique (and erase): ";
    for (int x : v) std::cout << x << " "; std::cout << "\n";

    // ranges::rotate
    std::ranges::rotate(v, v.begin() + 1);
    std::cout << "After ranges::rotate (left by 1): ";
    for (int x : v) std::cout << x << " "; std::cout << "\n";

    // ranges::sample
    std::vector<int> sampled;
    std::ranges::sample(v, std::back_inserter(sampled), 2, std::mt19937{std::random_device{}()});
    std::cout << "After ranges::sample (2 elements): ";
    for (int x : sampled) std::cout << x << " "; std::cout << "\n\n";

    // 2. Demonstration of custom transform_if
    std::vector<int> nums = {1, 2, 3, 4, 5, 6};
    auto even_squares = transform_if(nums, [](int n){ return n % 2 == 0; }, [](int n){ return n * n; });
    std::cout << "transform_if (squares of even): ";
    for (int x : even_squares) std::cout << x << " "; std::cout << "\n\n";

    // 3. MAE and MSE Calculation
    std::vector<double> y_true = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> y_pred = {1.1, 1.9, 3.2, 3.8};
    std::cout << "True values: [1.0, 2.0, 3.0, 4.0]\n";
    std::cout << "Predicted:   [1.1, 1.9, 3.2, 3.8]\n";
    std::cout << "MAE: " << calculate_mae(y_true, y_pred) << "\n";
    std::cout << "MSE: " << calculate_mse(y_true, y_pred) << "\n\n";

    // 4. Demonstration of views
    std::cout << "Views Demos:\n";
    
    // views::filter, views::drop
    auto filtered_dropped = nums | std::views::filter([](int n){ return n > 2; }) | std::views::drop(2);
    std::cout << "Filter (>2) -> Drop(2): ";
    for (int x : filtered_dropped) std::cout << x << " "; std::cout << "\n";

    // views::join
    std::vector<std::vector<int>> nested = {{1, 2}, {3, 4}, {5}};
    auto joined = nested | std::views::join;
    std::cout << "Joined nested vector: ";
    for (int x : joined) std::cout << x << " "; std::cout << "\n";

    // views::zip
    auto zipped = std::views::zip(nums, std::views::reverse(nums));
    std::cout << "Zip (nums, reversed nums): ";
    for (auto [a, b] : zipped) std::cout << "(" << a << "," << b << ") "; std::cout << "\n";

    // views::stride workaround (as std::views::stride is C++23 and may be missing in some libc++ versions)
    auto strided = std::views::zip(nums, std::views::iota(0))
        | std::views::filter([](const auto& p) { return std::get<1>(p) % 2 == 0; })
        | std::views::transform([](const auto& p) { return std::get<0>(p); });
    std::cout << "Stride(2) workaround of nums: ";
    for (int x : strided) std::cout << x << " "; std::cout << "\n";

    return 0;
}
