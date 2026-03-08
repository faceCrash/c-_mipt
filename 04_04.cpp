#include <iostream>
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <utility>

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Vector
{
public:

    Vector() : m_array(nullptr), m_size(0), m_capacity(0)
    {
        std::cout << "Vector::Vector (1)\n";
    }

    // -------------------------------------------------

    Vector(std::initializer_list<T> list)
        : m_size(list.size()), m_capacity(list.size())
    {
        std::cout << "Vector::Vector (2)\n";

        m_array = m_capacity ? new T[m_capacity]{} : nullptr;

        std::copy(list.begin(), list.end(), m_array);
    }

    // -------------------------------------------------

    Vector(const Vector& other)
        : m_size(other.m_size), m_capacity(other.m_capacity)
    {
        std::cout << "Vector::Vector (3)\n";

        m_array = m_capacity ? new T[m_capacity]{} : nullptr;

        std::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

    // -------------------------------------------------

    Vector(Vector&& other) noexcept
        : m_array(other.m_array),
          m_size(other.m_size),
          m_capacity(other.m_capacity)
    {
        std::cout << "Vector::Vector (4)\n";

        other.m_array = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // -------------------------------------------------

    ~Vector()
    {
        std::cout << "Vector::~Vector\n";
        delete[] m_array;
    }

    // -------------------------------------------------

    Vector& operator=(Vector other)
    {
        std::cout << "Vector::operator= (copy-swap)\n";
        swap(other);
        return *this;
    }

    // -------------------------------------------------

    void swap(Vector& other)
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

    // -------------------------------------------------

    void push_back(const T& value)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        m_array[m_size++] = value;
    }

    void push_back(T&& value)
    {
        if (m_size >= m_capacity)
            reserve(m_capacity == 0 ? 1 : m_capacity * 2);

        m_array[m_size++] = std::move(value);
    }

    // -------------------------------------------------

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= m_capacity)
            return;

        T* new_array = new T[new_capacity]{};

        std::move(m_array, m_array + m_size, new_array);

        delete[] m_array;
        m_array = new_array;
        m_capacity = new_capacity;
    }

    // -------------------------------------------------

    void clear()
    {
        std::cout << "Vector::clear\n";
        m_size = 0;
    }

    bool empty() const
    {
        return m_size == 0;
    }

    std::size_t size() const
    {
        return m_size;
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

    // -------------------------------------------------

    T& operator[](std::size_t index)
    {
        return m_array[index];
    }

    const T& operator[](std::size_t index) const
    {
        return m_array[index];
    }

private:
    T* m_array;
    std::size_t m_size;
    std::size_t m_capacity;
};

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs)
{
    lhs.swap(rhs);
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);
    v1.push_back(30);

    std::cout << "Size = " << v1.size() << ", Capacity = " << v1.capacity() << "\n";

    v1.push_back(40);
    v1.push_back(50);

    std::cout << "After adding 2 elements: Size = " << v1.size()
              << ", Capacity = " << v1.capacity() << "\n";

    for (std::size_t i = 0; i < v1.size(); ++i)
        std::cout << "v1[" << i << "] = " << v1[i] << "\n";

    v1.clear();
    std::cout << "After clear: Size = " << v1.size()
              << ", Empty = " << std::boolalpha << v1.empty() << "\n";

    // -------------------------------------------------
    Vector<std::string> v2 = {"Hello", "World"};
    v2.push_back("Template");
    for (std::size_t i = 0; i < v2.size(); ++i)
        std::cout << "v2[" << i << "] = " << v2[i] << "\n";

    return 0;
}
