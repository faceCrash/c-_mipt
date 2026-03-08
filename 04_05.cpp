#include <limits>
#include <type_traits>

// Базовый шаблон для вычисления N-го числа Фибоначчи
template<int N>
struct Fibonacci {
    static_assert(N > 0, "N должно быть больше 0");

    // Вычисляем предыдущее и текущее число рекурсивно
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;

    // Проверка переполнения типа int
    static_assert(value > 0, "Переполнение int при вычислении числа Фибоначчи");
};

// Специализация для первого числа ряда Фибоначчи
template<>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

// Специализация для второго числа ряда Фибоначчи
template<>
struct Fibonacci<2> {
    static constexpr int value = 1;
};

// Шаблон псевдонима для сокращения записи
template<int N>
constexpr int fibonacci_v = Fibonacci<N>::value;

// Тесты на этапе компиляции (static_assert)
static_assert(fibonacci_v<1> == 1, "Fibonacci<1> должен быть 1");
static_assert(fibonacci_v<2> == 1, "Fibonacci<2> должен быть 1");
static_assert(fibonacci_v<3> == 2, "Fibonacci<3> должен быть 2");
static_assert(fibonacci_v<4> == 3, "Fibonacci<4> должен быть 3");
static_assert(fibonacci_v<10> == 55, "Fibonacci<10> должен быть 55");

// Еще несколько тестов
static_assert(fibonacci_v<5> == 5, "Fibonacci<5> должен быть 5");
static_assert(fibonacci_v<6> == 8, "Fibonacci<6> должен быть 8");
static_assert(fibonacci_v<7> == 13, "Fibonacci<7> должен быть 13");
static_assert(fibonacci_v<8> == 21, "Fibonacci<8> должен быть 21");
static_assert(fibonacci_v<9> == 34, "Fibonacci<9> должен быть 34");

int main() {
    // Можно вывести для проверки на этапе выполнения
    constexpr int fib10 = fibonacci_v<10>;
    return fib10; // значение равно 55
}
