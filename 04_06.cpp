#include <iostream>

// constexpr-функция для вычисления e с точностью epsilon
constexpr double compute_e(double epsilon, int k = 0, double term = 1.0, double sum = 0.0)
{
    return (term < epsilon)
        ? sum
        : compute_e(epsilon, k + 1, term / static_cast<double>(k + 1), sum + term);
}

int main()
{
    constexpr double epsilon = 1e-10;
    constexpr double e = compute_e(epsilon); // вычисляем e при компиляции

    std::cout << "e ≈ " << e << '\n';
    return 0;
}
