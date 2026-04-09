#include <iostream>
#include <iterator>
#include <boost/iterator/iterator_facade.hpp>

/**
 * @brief Standard implementation of Fibonacci forward iterator.
 */
class FibIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    FibIterator(int a = 0, int b = 1) : m_a(a), m_b(b) {}

    int operator*() const { return m_a; }

    FibIterator& operator++() {
        int next = m_a + m_b;
        m_a = m_b;
        m_b = next;
        return *this;
    }

    FibIterator operator++(int) {
        FibIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    bool operator==(const FibIterator& other) const {
        return m_a == other.m_a && m_b == other.m_b;
    }
    bool operator!=(const FibIterator& other) const {
        return !(*this == other);
    }

private:
    int m_a, m_b;
};

/**
 * @brief Fibonacci Iterator using Boost.Iterator (facade).
 */
class FibIteratorBoost : public boost::iterator_facade<
    FibIteratorBoost,
    int,
    boost::forward_traversal_tag,
    int // Read-only value access
> {
public:
    FibIteratorBoost(int a = 0, int b = 1) : m_a(a), m_b(b) {}

private:
    friend class boost::iterator_core_access;

    void increment() {
        int next = m_a + m_b;
        m_a = m_b;
        m_b = next;
    }

    bool equal(const FibIteratorBoost& other) const {
        return m_a == other.m_a && m_b == other.m_b;
    }

    int dereference() const { return m_a; }

    int m_a, m_b;
};

int main() {
    std::cout << "Standard FibIterator (first 10): ";
    FibIterator it(0, 1);
    for (int i = 0; i < 10; ++i, ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Boost FibIterator (first 10):    ";
    FibIteratorBoost itb(0, 1);
    for (int i = 0; i < 10; ++i, ++itb) {
        std::cout << *itb << " ";
    }
    std::cout << "\n";

    return 0;
}
