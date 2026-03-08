#include <iostream>
#include <vector>
#include <cstddef> // для std::size_t

int main() {
    const unsigned long long int limit = 100; // верхняя граница последовательностей
    std::vector<std::size_t> collatz_lengths(limit + 1, 0); // кэш для длин последовательностей

    std::size_t max_length = 0;      // максимальная длина последовательности
    unsigned long long int number = 0; // число с максимальной длиной

    // Проходим по всем числам от 1 до limit
    for (unsigned long long int i = 1; i <= limit; ++i) {
        unsigned long long int n = i;
        std::size_t length = 0;

        // Вычисляем длину последовательности для n
        while (n != 1 && (n >= collatz_lengths.size() || collatz_lengths[n] == 0)) {
            ++length;
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = 3 * n + 1;
            }
        }

        // Добавляем длину из кэша, если дошли до известного числа
        if (n < collatz_lengths.size()) {
            length += collatz_lengths[n];
        }

        // Сохраняем длину в кэш
        if (i < collatz_lengths.size()) {
            collatz_lengths[i] = length;
        }

        // Проверяем, является ли эта последовательность максимальной
        if (length > max_length) {
            max_length = length;
            number = i;
        }
    }

    // Вывод результата
    std::cout << "Число с наибольшей длиной последовательности Коллатца: " << number << "\n";
    std::cout << "Длина последовательности: " << max_length << "\n";

    return 0;
}
