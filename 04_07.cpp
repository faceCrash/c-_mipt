#include <cassert>

//////////////////////////////////////////////////////////////////////////////////////////

template <int N = 0, int D = 1>
struct Ratio
{
    // Числитель дроби
    constexpr static auto num = N;
    // Знаменатель дроби  
    constexpr static auto den = D;
};

// Функция для вычисления НОД (так как std::gcd требует C++17)
constexpr int compute_gcd(int a, int b)
{
    // Работаем с абсолютными значениями для отрицательных чисел
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    return (b == 0) ? a : compute_gcd(b, a % b);
}

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sum
{
private:
    // Вычисление числителя и знаменателя без сокращения
    constexpr static auto raw_num = R1::num * R2::den + R2::num * R1::den;
    constexpr static auto raw_den = R1::den * R2::den;
    
    // Сокращение дроби с помощью НОД
    constexpr static auto gcd_val = compute_gcd(raw_num, raw_den);

public:
    // Сокращенные значения
    constexpr static auto num = raw_num / gcd_val;
    constexpr static auto den = raw_den / gcd_val;
    
    // Тип сокращенной дроби
    using type = Ratio<num, den>;
};

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
using sum = typename Sum<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Sub
{
    // Вычитание дробей через сложение с отрицательной дробью
    using type = typename Sum<R1, Ratio<-R2::num, R2::den>>::type;
    
    // Поля для удобства тестирования
    constexpr static auto num = type::num;
    constexpr static auto den = type::den;
};

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
using sub = typename Sub<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Mul
{
private:
    // Вычисление числителя и знаменателя без сокращения
    constexpr static auto raw_num = R1::num * R2::num;
    constexpr static auto raw_den = R1::den * R2::den;
    
    // Сокращение дроби с помощью НОД
    constexpr static auto gcd_val = compute_gcd(raw_num, raw_den);

public:
    // Сокращенные значения
    constexpr static auto num = raw_num / gcd_val;
    constexpr static auto den = raw_den / gcd_val;
    
    // Тип сокращенной дроби
    using type = Ratio<num, den>;
};

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
using mul = typename Mul<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename R1, typename R2>
struct Div
{
    // Проверка деления на ноль
    static_assert(R2::num != 0, "Division by zero!");
    
    // Деление дробей через умножение на обратную дробь
    using type = typename Mul<R1, Ratio<R2::den, R2::num>>::type;
    
    // Поля для удобства тестирования
    constexpr static auto num = type::num;
    constexpr static auto den = type::den;
};

//////////////////////////////////////////////////////////////////////////////////////////

// Изменяем имя, чтобы избежать конфликта с std::div
template <typename R1, typename R2>
using divide = typename Div<R1, R2>::type;

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T, typename R = Ratio<1>>
struct Duration
{
    T value;  // Значение длительности
    
    // Конструктор для инициализации значения
    constexpr Duration(T v = T{}) : value(v) {}
};

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator+(const Duration<T1, R1>& lhs, const Duration<T2, R2>& rhs)
{
    // Вычисление общего знаменателя для сложения
    using result_ratio = sum<R1, R2>;
    
    // Вычисление значения суммы
    auto result_value = 
        lhs.value * result_ratio::den / R1::den * R1::num +
        rhs.value * result_ratio::den / R2::den * R2::num;
    
    return Duration<decltype(result_value), result_ratio>(result_value);
}

//////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename R1, typename T2, typename R2>
constexpr auto operator-(const Duration<T1, R1>& lhs, const Duration<T2, R2>& rhs)
{
    // Использование оператора сложения для реализации вычитания
    using negative_ratio = Ratio<-R2::num, R2::den>;
    return lhs + Duration<T2, negative_ratio>(rhs.value);
}

//////////////////////////////////////////////////////////////////////////////////////////

// Тесты для операций с дробями

// Сложение: 1/2 + 1/3 = 3/6 + 2/6 = 5/6
static_assert(Sum<Ratio<1, 2>, Ratio<1, 3>>::num == 5, "Sum test 1 failed");
static_assert(Sum<Ratio<1, 2>, Ratio<1, 3>>::den == 6, "Sum test 2 failed");

// Вычитание: 1/2 - 1/3 = 3/6 - 2/6 = 1/6
static_assert(Sub<Ratio<1, 2>, Ratio<1, 3>>::num == 1, "Sub test 1 failed");
static_assert(Sub<Ratio<1, 2>, Ratio<1, 3>>::den == 6, "Sub test 2 failed");

// Умножение: 1/2 * 2/3 = 2/6 = 1/3
static_assert(Mul<Ratio<1, 2>, Ratio<2, 3>>::num == 1, "Mul test 1 failed");
static_assert(Mul<Ratio<1, 2>, Ratio<2, 3>>::den == 3, "Mul test 2 failed");

// Деление: 1/2 ÷ 2/3 = 1/2 * 3/2 = 3/4
static_assert(Div<Ratio<1, 2>, Ratio<2, 3>>::num == 3, "Div test 1 failed");
static_assert(Div<Ratio<1, 2>, Ratio<2, 3>>::den == 4, "Div test 2 failed");

// Тесты для псевдонимов (тестируем через их тип)
static_assert(sum<Ratio<1, 4>, Ratio<1, 4>>::num == 1, "sum alias test 1 failed");
static_assert(sum<Ratio<1, 4>, Ratio<1, 4>>::den == 2, "sum alias test 2 failed");

static_assert(sub<Ratio<3, 4>, Ratio<1, 4>>::num == 1, "sub alias test 1 failed");
static_assert(sub<Ratio<3, 4>, Ratio<1, 4>>::den == 2, "sub alias test 2 failed");

static_assert(mul<Ratio<1, 2>, Ratio<2, 3>>::num == 1, "mul alias test 1 failed");
static_assert(mul<Ratio<1, 2>, Ratio<2, 3>>::den == 3, "mul alias test 2 failed");

static_assert(divide<Ratio<1, 2>, Ratio<2, 3>>::num == 3, "divide alias test 1 failed");
static_assert(divide<Ratio<1, 2>, Ratio<2, 3>>::den == 4, "divide alias test 2 failed");

// Тест на сокращение дробей: 2/4 + 2/6 = 1/2 + 1/3 = 3/6 + 2/6 = 5/6
static_assert(Sum<Ratio<2, 4>, Ratio<2, 6>>::num == 5, "Reduction test 1 failed");
static_assert(Sum<Ratio<2, 4>, Ratio<2, 6>>::den == 6, "Reduction test 2 failed");

// Дополнительные тесты операций
static_assert(mul<Ratio<3, 4>, Ratio<2, 5>>::num == 3, "Multiplication test 1 failed");
static_assert(mul<Ratio<3, 4>, Ratio<2, 5>>::den == 10, "Multiplication test 2 failed");

static_assert(divide<Ratio<3, 4>, Ratio<2, 5>>::num == 15, "Division test 1 failed");
static_assert(divide<Ratio<3, 4>, Ratio<2, 5>>::den == 8, "Division test 2 failed");

// Тест на вычитание с отрицательными числами: 1/4 - 3/4 = -2/4 = -1/2
static_assert(Sub<Ratio<1, 4>, Ratio<3, 4>>::num == -1, "Negative subtraction test 1 failed");
static_assert(Sub<Ratio<1, 4>, Ratio<3, 4>>::den == 2, "Negative subtraction test 2 failed");

// Тест на сложение с отрицательными числами: -1/2 + 1/2 = 0
static_assert(Sum<Ratio<-1, 2>, Ratio<1, 2>>::num == 0, "Negative addition test failed");

//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // Тестирование операций с длительностями
    
    // Исходные данные
    constexpr auto x = 1, y = 2;
    
    // Создание длительностей с разными коэффициентами
    constexpr Duration<int, Ratio<1, 2>> duration_1(x);
    constexpr Duration<int, Ratio<1, 3>> duration_2(y);
    
    // Тестирование сложения
    constexpr auto duration_3 = duration_1 + duration_2;
    static_assert(duration_3.value == 7, "Addition test failed");
    
    // Тестирование вычитания
    constexpr auto duration_4 = duration_1 - duration_2;
    static_assert(duration_4.value == -1, "Subtraction test failed");
    
    // Дополнительные тесты с другими коэффициентами
    constexpr Duration<int, Ratio<2, 3>> duration_5(3);
    constexpr Duration<int, Ratio<1, 4>> duration_6(2);
    
    // Тестирование сложения с другими коэффициентами
    constexpr auto duration_7 = duration_5 + duration_6;
    
    // Тестирование вычитания с другими коэффициентами  
    constexpr auto duration_8 = duration_5 - duration_6;
    
    // Проверка во время выполнения (для демонстрации)
    assert(duration_3.value == 7);
    assert(duration_4.value == -1);
    
    return 0;
}