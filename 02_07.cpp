#include <iostream>
#include <cmath>       // для std::sqrt
#include <limits>      // для std::numeric_limits

int main() {
    std::size_t size;
    std::cout << "Введите количество чисел: ";
    std::cin >> size;

    // Создаем динамический массив
    double* numbers = new double[size];

    // Ввод чисел
    std::cout << "Введите " << size << " чисел: ";
    for (std::size_t i = 0; i < size; ++i) {
        std::cin >> numbers[i];
    }

    // Инициализация переменных
    double max_value = std::numeric_limits<double>::lowest();
    double min_value = std::numeric_limits<double>::max();
    double sum = 0.0;

    // Вычисляем max, min и сумму
    for (std::size_t i = 0; i < size; ++i) {
        if (numbers[i] > max_value) max_value = numbers[i];
        if (numbers[i] < min_value) min_value = numbers[i];
        sum += numbers[i];
    }

    double mean = sum / size; // среднее арифметическое

    // Вычисление стандартного отклонения
    double variance_sum = 0.0;
    for (std::size_t i = 0; i < size; ++i) {
        variance_sum += (numbers[i] - mean) * (numbers[i] - mean);
    }
    double std_dev = std::sqrt(variance_sum / size);

    // Вывод результатов
    std::cout << "Максимальное значение: " << max_value << "\n";
    std::cout << "Минимальное значение: " << min_value << "\n";
    std::cout << "Среднее значение: " << mean << "\n";
    std::cout << "Стандартное отклонение: " << std_dev << "\n";

    // Освобождение памяти
    delete[] numbers;

    return 0;
}
