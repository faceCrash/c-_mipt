// 02_02_quadratic_roots.cpp
// Вычисление корней квадратного уравнения a*x^2 + b*x + c = 0
// Вход: коэффициенты a b c (std::cin)
// Вывод: корни или сообщения о количестве корней (std::cout)
// Точность сравнения с нулём задаётся константой epsilon.

#include <iostream>
#include <cmath>
#include <limits>

int main() {
    double a = 0.0, b = 0.0, c = 0.0;
    if (!(std::cin >> a >> b >> c)) {
        return 1; // ошибка ввода
    }

    // Порог точности для сравнения с нулём
    const double epsilon = 1e-12;

    // Если a "приблизительно" ноль, уравнение вырождается в линейное bx + c = 0
    if (std::abs(a) <= epsilon) {
        if (std::abs(b) <= epsilon) {
            // a ≈ 0, b ≈ 0 => c ≈ 0 => либо бесконечно много решений, либо нет решений
            if (std::abs(c) <= epsilon) {
                std::cout << "Infinite number of solutions\n";
            } else {
                std::cout << "No solutions\n";
            }
        } else {
            // Линейное уравнение bx + c = 0
            double x = -c / b;
            // Допускаем вывод отрицательного нуля (напр., -0.0) — как требует условие.
            std::cout << x << '\n';
        }
        return 0;
    }

    // Квадратное уравнение: вычисляем дискриминант
    double discriminant = b * b - 4.0 * a * c;

    if (discriminant > epsilon) {
        // Два различных корня
        double sqrt_d = std::sqrt(discriminant);
        double x1 = (-b + sqrt_d) / (2.0 * a);
        double x2 = (-b - sqrt_d) / (2.0 * a);
        std::cout << x1 << ' ' << x2 << '\n';
    } else if (std::abs(discriminant) <= epsilon) {
        // Двойной корень (один корень)
        double x = -b / (2.0 * a);
        std::cout << x << '\n';
    } else {
        // Дискриминант отрицательный — действительных корней нет
        std::cout << "No real roots\n";
    }

    return 0;
}
