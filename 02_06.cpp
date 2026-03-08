#include <iostream>
#include <cmath> // для std::sqrt
#include <limits> // для std::numeric_limits

int main() {
    const int SIZE = 5; // размер статического массива
    double numbers[SIZE]; // статический массив для хранения чисел

    // Ввод чисел
    std::cout << "Введите " << SIZE << " чисел: ";
    for (int i = 0; i < SIZE; ++i) {
        std::cin >> numbers[i];
    }

    // Инициализация переменных для вычислений
    double max_value = std::numeric_limits<double>::lowest(); // максимальное значение
    double min_value = std::numeric_limits<double>::max();    // минимальное значение
    double sum = 0.0;                                         // сумма для среднего

    // Вычисляем max, min и сумму
    for (int i = 0; i < SIZE; ++i) {
        if (numbers[i] > max_value) max_value = numbers[i];
        if (numbers[i] < min_value) min_value = numbers[i];
        sum += numbers[i];
    }

    double mean = sum / SIZE; // среднее арифметическое

    // Вычисление стандартного отклонения
    double variance_sum = 0.0;
    for (int i = 0; i < SIZE; ++i) {
        variance_sum += (numbers[i] - mean) * (numbers[i] - mean);
    }
    double std_dev = std::sqrt(variance_sum / SIZE);

    // Вывод результатов
    std::cout << "Максимальное значение: " << max_value << "\n";
    std::cout << "Минимальное значение: " << min_value << "\n";
    std::cout << "Среднее значение: " << mean << "\n";
    std::cout << "Стандартное отклонение: " << std_dev << "\n";

    return 0;
}
