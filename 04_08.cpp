#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

//////////////////////////////////////////////////////////////////

// Базовый шаблон для пустого кортежа
template <typename... Ts> 
class Tuple 
{
public:
    // Конструктор по умолчанию
    Tuple() = default;
    
    // Функция для получения размера кортежа
    constexpr std::size_t size() const noexcept
    {
        return sizeof...(Ts);
    }
};

//////////////////////////////////////////////////////////////////

// Вспомогательный шаблон для доступа к элементам
template <std::size_t I, typename TupleType>
struct TupleGetHelper;

// Специализация для пустого кортежа
template <std::size_t I>
struct TupleGetHelper<I, Tuple<>>
{
    // Пустой кортеж не имеет элементов
    static_assert(I == I, "Index out of bounds for empty tuple");
};

//////////////////////////////////////////////////////////////////

// Специализация для непустого кортежа
template <typename T, typename... Ts> 
class Tuple<T, Ts...>
{
private:
    T m_head;                // Головной элемент
    Tuple<Ts...> m_tail;     // Хвост кортежа

public:
    // Конструктор с perfect forwarding
    Tuple(T&& x, Ts&&... ys)
        : m_head(std::forward<T>(x)),
          m_tail(std::forward<Ts>(ys)...)
    {}

    // Функция для получения элемента по индексу
    template <std::size_t I> 
    auto get() const
    {
        return TupleGetHelper<I, Tuple<T, Ts...>>::get(*this);
    }

    // Функция для получения размера кортежа
    constexpr std::size_t size() const noexcept
    {
        return sizeof...(Ts) + 1;
    }

    // Объявляем вспомогательный класс дружественным для доступа к приватным членам
    template <std::size_t I2, typename TupleType>
    friend struct TupleGetHelper;
};

//////////////////////////////////////////////////////////////////

// Специализация для индекса 0
template <typename T, typename... Ts>
struct TupleGetHelper<0, Tuple<T, Ts...>>
{
    static auto get(const Tuple<T, Ts...>& tuple)
    {
        return tuple.m_head;
    }
};

// Специализация для индекса > 0
template <std::size_t I, typename T, typename... Ts>
struct TupleGetHelper<I, Tuple<T, Ts...>>
{
    static auto get(const Tuple<T, Ts...>& tuple)
    {
        static_assert(I > 0, "Index must be positive for recursive case");
        static_assert(I < sizeof...(Ts) + 1, "Index out of bounds");
        return TupleGetHelper<I - 1, Tuple<Ts...>>::get(tuple.m_tail);
    }
};

//////////////////////////////////////////////////////////////////

// Тест 1: Пустой кортеж
void test_empty_tuple()
{
    Tuple<> empty_tuple;
    assert(empty_tuple.size() == 0);
}

// Тест 2: Кортеж с одним элементом
void test_single_element_tuple()
{
    Tuple<int> single_tuple(42);
    assert(single_tuple.size() == 1);
    assert(single_tuple.get<0>() == 42);
}

// Тест 3: Кортеж с двумя элементами
void test_double_tuple()
{
    Tuple<int, double> double_tuple(1, 2.0);
    assert(double_tuple.size() == 2);
    assert(double_tuple.get<0>() == 1);
    assert(double_tuple.get<1>() == 2.0);
}

// Тест 4: Кортеж с тремя элементами (оригинальный тест)
void test_original_tuple()
{
    Tuple<int, double, std::string> tuple(1, 2.0, "aaaaa");
    assert(tuple.size() == 3);
    assert(tuple.get<0>() == 1);
    assert(tuple.get<1>() == 2.0);
    assert(tuple.get<2>() == "aaaaa");
}

// Тест 5: Кортеж с четырьмя элементами
void test_four_element_tuple()
{
    Tuple<int, double, char, float> four_tuple(10, 20.5, 'X', 3.14f);
    assert(four_tuple.size() == 4);
    assert(four_tuple.get<0>() == 10);
    assert(four_tuple.get<1>() == 20.5);
    assert(four_tuple.get<2>() == 'X');
    assert(four_tuple.get<3>() == 3.14f);
}

// Тест 6: Кортеж с разными типами
void test_mixed_types_tuple()
{
    Tuple<std::string, bool, int> mixed_tuple("test", true, 100);
    assert(mixed_tuple.size() == 3);
    assert(mixed_tuple.get<0>() == "test");
    assert(mixed_tuple.get<1>() == true);
    assert(mixed_tuple.get<2>() == 100);
}

// Тест 7: Кортеж с одинаковыми типами
void test_same_types_tuple()
{
    Tuple<int, int, int> same_type_tuple(1, 2, 3);
    assert(same_type_tuple.size() == 3);
    assert(same_type_tuple.get<0>() == 1);
    assert(same_type_tuple.get<1>() == 2);
    assert(same_type_tuple.get<2>() == 3);
}

// Тест 8: Константный кортеж
void test_const_tuple()
{
    const Tuple<int, double> const_tuple(5, 10.5);
    assert(const_tuple.size() == 2);
    assert(const_tuple.get<0>() == 5);
    assert(const_tuple.get<1>() == 10.5);
}

// Тест 9: Кортеж с перемещением
void test_move_tuple()
{
    std::string str = "movable";
    Tuple<std::string> moved_tuple(std::move(str));
    assert(moved_tuple.size() == 1);
    assert(moved_tuple.get<0>() == "movable");
}

// Тест 10: Кортеж с вложенными кортежами
void test_nested_tuple()
{
    Tuple<int, Tuple<double, char>> nested_tuple(1, Tuple<double, char>(2.0, 'A'));
    assert(nested_tuple.size() == 2);
    assert(nested_tuple.get<0>() == 1);
    assert(nested_tuple.get<1>().get<0>() == 2.0);
    assert(nested_tuple.get<1>().get<1>() == 'A');
}

//////////////////////////////////////////////////////////////////

int main()
{
    // Запуск всех тестов
    test_empty_tuple();
    test_single_element_tuple();
    test_double_tuple();
    test_original_tuple();
    test_four_element_tuple();
    test_mixed_types_tuple();
    test_same_types_tuple();
    test_const_tuple();
    test_move_tuple();
    test_nested_tuple();
    
    return 0;
}