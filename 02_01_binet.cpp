// 02_01_binet.cpp
// Вычисление N-ого числа ряда Фибоначчи по формуле Бине.
// Вход: целое N (std::cin).
// Вывод: Fibonacci(N) (std::cout).
// Примечание: используется double для промежуточных вычислений,
// конечный результат приводится к int через static_cast<int>.

// Формула Бине:
// F(n) = (phi^n - psi^n) / sqrt(5),
// где phi = (1 + sqrt(5)) / 2, psi = (1 - sqrt(5)) / 2.
// Обоснование: последовательность Фибоначчи имеет характеристическое
// уравнение r^2 = r + 1 => r = (1 ± sqrt(5)) / 2.
// Общий вид решения линейной рекуррентной последовательности
// даёт линейную комбинацию phi^n и psi^n; подставляя начальные условия
// получаем коэффициенты, в результате — формула Бине.
// Поскольку |psi| < 1, psi^n стремится к 0 при больших n, но точное
// значение учитывается для небольших n; используем std::round для
// компенсации ошибок округления.

#include <iostream>
#include <cmath>
#include <limits>

int main() {
    // Читаем N
    long long N = 0;
    if (!(std::cin >> N)) {
        // Ошибка ввода — выходим с кодом 1
        return 1;
    }

    // Обработка неотрицательности N (в задаче предполагается N >= 0)
    if (N < 0) {
        // Негативные индексы не поддерживаются в этой реализации.
        std::cout << "Error: negative index\n";
        return 0;
    }

    // Константы формулы Бине
    const double sqrt5 = std::sqrt(5.0);
    const double phi = (1.0 + sqrt5) / 2.0;
    const double psi = (1.0 - sqrt5) / 2.0;

    // Вычисляем phi^N и psi^N с использованием std::pow
    double phi_pow = std::pow(phi, static_cast<double>(N));
    double psi_pow = std::pow(psi, static_cast<double>(N));

    // Формула Бине (double). Затем округление и приведение к int.
    double fib_approx = (phi_pow - psi_pow) / sqrt5;
    // Округлим до ближайшего целого, затем приведём к int.
    // Используем std::round для корректного округления double -> ближайшее целое.
    int fib_int = static_cast<int>(std::round(fib_approx));

    std::cout << fib_int << '\n';
    return 0;
}