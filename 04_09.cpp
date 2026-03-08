#include <cstddef>

template <typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
};

using true_type  = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <typename T>
struct is_class : integral_constant<bool, __is_class(T)> {};

template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template <typename T>
struct remove_const { using type = T; };

template <typename T>
struct remove_const<const T> { using type = T; };

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct add_const { using type = const T; };

template <typename T>
using add_const_t = typename add_const<T>::type;

template <bool B, typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T; };

template <typename T>
struct remove_reference<T&&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <typename T>
struct is_array : false_type {};

template <typename T, std::size_t N>
struct is_array<T[N]> : true_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T>
struct remove_extent { using type = T; };

template <typename T, std::size_t N>
struct remove_extent<T[N]> { using type = T; };

template <typename T>
struct remove_extent<T[]> { using type = T; };

template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

template <typename>
struct is_function : false_type {};

template <typename R, typename... Args>
struct is_function<R(Args...)> : true_type {};

template <typename R, typename... Args>
struct is_function<R(Args..., ...)> : true_type {};

template <typename T>
struct add_pointer { using type = T*; };

template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

template <typename T>
struct decay
{
private:
    using U = remove_reference_t<T>;

public:
    using type =
        conditional_t<
            is_array<U>::value,
            add_pointer_t<remove_extent_t<U>>,
            conditional_t<
                is_function<U>::value,
                add_pointer_t<U>,
                remove_const_t<U>
            >
        >;
};

template <typename T>
using decay_t = typename decay<T>::type;

struct A {};
class  B {};
union  C { int x; };

static_assert(is_class_v<A>);
static_assert(is_class_v<B>);
static_assert(!is_class_v<C>);
static_assert(!is_class_v<int>);
static_assert(!is_class_v<void>);

static_assert(is_same_v<add_const_t<int>, const int>);
static_assert(is_same_v<remove_const_t<const int>, int>);

static_assert(is_same_v<conditional_t<true, int, double>, int>);
static_assert(is_same_v<conditional_t<false, int, double>, double>);

static_assert(is_same_v<decay_t<int>, int>);
static_assert(is_same_v<decay_t<const int>, int>);
static_assert(is_same_v<decay_t<int&>, int>);
static_assert(is_same_v<decay_t<int[3]>, int*>);
static_assert(is_same_v<decay_t<int(int)>, int(*)(int)>);

int main() {}
