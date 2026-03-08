#include <iostream>
#include <type_traits>
#include <limits>

// ================== Сумма ==================
template<typename... Args>
double sum(Args&&... args) {
    double result = 0.0;
    (void)std::initializer_list<int>{
        (std::is_same<double, typename std::decay<Args>::type>::value ? (result += args, 0) : 0)...};
    return result;
}

// ================== Среднее ==================
template<typename... Args>
double average(Args&&... args) {
    double result = 0.0;
    int count = 0;
    (void)std::initializer_list<int>{
        (std::is_same<double, typename std::decay<Args>::type>::value ? (result += args, ++count, 0) : 0)...};
    return count > 0 ? result / count : 0.0;
}

// ================== Минимум ==================
template<typename... Args>
double min_val(Args&&... args) {
    double minV = std::numeric_limits<double>::max();
    (void)std::initializer_list<int>{
        (std::is_same<double, typename std::decay<Args>::type>::value ? (minV = (args < minV ? args : minV), 0) : 0)...};
    return minV;
}

// ================== Максимум ==================
template<typename... Args>
double max_val(Args&&... args) {
    double maxV = std::numeric_limits<double>::lowest();
    (void)std::initializer_list<int>{
        (std::is_same<double, typename std::decay<Args>::type>::value ? (maxV = (args > maxV ? args : maxV), 0) : 0)...};
    return maxV;
}

// ================== Тестирование ==================
int main() {
    double s = sum(1.0, 2, 3.5, 4.0f, 5.0);
    double avg = average(1.0, 2, 3.5, 4.0f, 5.0);
    double mn = min_val(1.0, 2, 3.5, 4.0f, 5.0);
    double mx = max_val(1.0, 2, 3.5, 4.0f, 5.0);

    std::cout << "Sum: " << s << "\n";      // 9.5
    std::cout << "Average: " << avg << "\n"; // 3.16667
    std::cout << "Min: " << mn << "\n";      // 1.0
    std::cout << "Max: " << mx << "\n";      // 5.0

    return 0;
}
