#include <iostream>

// Рекурсивная функция для вычисления наибольшего общего делителя (НОД)
int gcd(int a, int b) {
    return (b == 0) ? a : gcd(b, a % b);
}

// Функция для вычисления наименьшего общего делителя больше 1
int smallest_common_divisor(int a, int b) {
    int min_val = (a < b) ? a : b;
    for (int i = 2; i <= min_val; ++i) {
        if (a % i == 0 && b % i == 0) {
            return i; // Первый общий делитель > 1
        }
    }
    return -1; // Если общего делителя > 1 нет
}

// Функция для вычисления наименьшего общего кратного (НОК)
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

int main() {
    int a, b;

    std::cout << "Введите два натуральных числа через пробел: ";
    std::cin >> a >> b;

    if (a <= 0 || b <= 0) {
        std::cout << "Ошибка: числа должны быть натуральными.\n";
        return 1;
    }

    int result_gcd = gcd(a, b);
    std::cout << "НОД(" << a << ", " << b << ") = " << result_gcd << "\n";

    int result_scd = smallest_common_divisor(a, b);
    if (result_scd != -1) {
        std::cout << "Наименьший общий делитель > 1: " << result_scd << "\n";
    } else {
        std::cout << "Общего делителя > 1 нет\n";
    }

    int result_lcm = lcm(a, b);
    std::cout << "НОК(" << a << ", " << b << ") = " << result_lcm << "\n";

    return 0;
}
