#include <iostream>
#include <vector>
#include <functional> // для std::less

// Функция для разделения массива (pivot - последний элемент)
template<typename T, typename Compare = std::less<T>>
int partition(std::vector<T>& arr, int low, int high, Compare comp = Compare()) {
    T pivot = arr[high]; // опорный элемент
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (comp(arr[j], pivot) || arr[j] == pivot) { // меньше или равно
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Рекурсивная быстрая сортировка
template<typename T, typename Compare = std::less<T>>
void quickSort(std::vector<T>& arr, int low, int high, Compare comp = Compare()) {
    if (low < high) {
        int pi = partition(arr, low, high, comp);
        quickSort(arr, low, pi - 1, comp);
        quickSort(arr, pi + 1, high, comp);
    }
}

int main() {
    std::vector<int> arr = {34, 7, 23, 32, 5, 62, 32, 7, 0};

    std::cout << "Исходный массив: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    quickSort(arr, 0, arr.size() - 1);

    std::cout << "Отсортированный массив: ";
    for (int x : arr) std::cout << x << " ";
    std::cout << "\n";

    // Пример с типом double
    std::vector<double> arr_d = {3.4, 1.2, 5.6, 0.1};
    quickSort(arr_d, 0, arr_d.size() - 1);

    std::cout << "Отсортированный массив double: ";
    for (double x : arr_d) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
