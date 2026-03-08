#include <cstddef>
#include <type_traits>

template <typename... Ts>
struct Deque {};

template <typename D>
struct Size {};

template <typename... Ts>
struct Size<Deque<Ts...>>
{
    static constexpr std::size_t value = sizeof...(Ts);
};

template <typename D>
constexpr std::size_t size_v = Size<D>::value;

template <typename D>
constexpr bool is_empty_v = size_v<D> == 0;

template <typename D>
struct Front {};

template <typename T, typename... Ts>
struct Front<Deque<T, Ts...>>
{
    using type = T;
};

template <typename D>
using front = typename Front<D>::type;

template <typename T, typename D>
struct Push_Front {};

template <typename T, typename... Ts>
struct Push_Front<T, Deque<Ts...>>
{
    using type = Deque<T, Ts...>;
};

template <typename T, typename D>
using push_front = typename Push_Front<T, D>::type;

template <typename D>
struct Pop_Front {};

template <typename T, typename... Ts>
struct Pop_Front<Deque<T, Ts...>>
{
    using type = Deque<Ts...>;
};

template <typename D>
using pop_front = typename Pop_Front<D>::type;

template <typename D>
struct Back {};

template <typename T>
struct Back<Deque<T>>
{
    using type = T;
};

template <typename T, typename... Ts>
struct Back<Deque<T, Ts...>>
{
    using type = typename Back<Deque<Ts...>>::type;
};

template <typename D>
using back = typename Back<D>::type;

template <typename T, typename D, bool C = is_empty_v<D>>
struct Push_Back {};

template <typename T, typename D>
struct Push_Back<T, D, false>
{
    using type =
        push_front<
            front<D>,
            typename Push_Back<T, pop_front<D>>::type>;
};

template <typename T, typename D>
struct Push_Back<T, D, true>
{
    using type = push_front<T, D>;
};

template <typename T, typename D>
using push_back = typename Push_Back<T, D>::type;

template <typename D>
struct Pop_Back {};

template <typename T>
struct Pop_Back<Deque<T>>
{
    using type = Deque<>;
};

template <typename T, typename... Ts>
struct Pop_Back<Deque<T, Ts...>>
{
    using type =
        push_front<T, typename Pop_Back<Deque<Ts...>>::type>;
};

template <typename D>
using pop_back = typename Pop_Back<D>::type;

template <typename D, std::size_t I>
struct Nth : Nth<pop_front<D>, I - 1> {};

template <typename D>
struct Nth<D, 0> : Front<D> {};

template <typename D, std::size_t I>
using nth = typename Nth<D, I>::type;

template <typename D, bool C = is_empty_v<D>>
struct Max_Type {};

template <typename D>
struct Max_Type<D, false>
{
private:
    using current_t = front<D>;
    using max_t = typename Max_Type<pop_front<D>>::type;

public:
    using type =
        std::conditional_t<
            (sizeof(current_t) >= sizeof(max_t)),
            current_t,
            max_t>;
};

template <typename D>
struct Max_Type<D, true>
{
    using type = std::byte;
};

template <typename D>
using max_type = typename Max_Type<D>::type;

template <typename T, typename D, bool C = is_empty_v<D>>
struct Has {};

template <typename T, typename D>
struct Has<T, D, true>
{
    static constexpr bool value = false;
};

template <typename T, typename D>
struct Has<T, D, false>
{
    static constexpr bool value =
        std::is_same_v<T, front<D>> ||
        Has<T, pop_front<D>>::value;
};

template <typename T, typename D>
constexpr bool has_v = Has<T, D>::value;

int main()
{
    using D0 = Deque<>;
    using D1 = Deque<int>;
    using D2 = Deque<int, double, char>;

    static_assert(size_v<D0> == 0 && is_empty_v<D0>);
    static_assert(size_v<D1> == 1 && !is_empty_v<D1>);

    static_assert(std::is_same_v<front<D2>, int>);
    static_assert(std::is_same_v<back<D2>, char>);
    static_assert(std::is_same_v<nth<D2, 1>, double>);

    static_assert(std::is_same_v<max_type<Deque<int, double>>, double>);

    static_assert(!has_v<int, D0>);
    static_assert(has_v<int, D1>);
    static_assert(has_v<double, D2>);
    static_assert(has_v<char, D2>);
    static_assert(!has_v<float, D2>);
}