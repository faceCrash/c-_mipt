// 02_05_e_series.cpp
// Вычисление числа e = sum_{k=0..inf} 1 / k! с точностью epsilon.
// Вход: epsilon (std::cin).
// Выход: приближённое значение e (std::cout).
// Подход: не вычисляем факториалы напрямую (во избежание переполнения),
// используем рекуррентную формулу term_k = term_{k-1} / k.

#include <iostream>
#include <cmath>

int main() {
    double epsilon = 0.0;
    if (!(std::cin >> epsilon)) {
        return 1; // ошибка ввода
    }
    if (epsilon <= 0.0) {
        std::cout << "Error: epsilon must be positive\n";
        return 0;
    }

    double sum = 0.0;
    double term = 1.0; // term_0 = 1 / 0! = 1
    std::size_t k = 0;

    // Добавляем члены ряда до тех пор, пока очередной член не станет < epsilon
    while (term >= epsilon) {
        sum += term;
        ++k;
        term = term / static_cast<double>(k); // term_k = term_{k-1} / k
    }

    std::cout << sum << '\n';
    return 0;
}
