#include <compare>
#include <numeric>
#include <cassert>
#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational
{
public:
    Rational(int num = 0, int den = 1)
        : m_num(num), m_den(den)
    {
        reduce();
    }

    explicit operator double() const
    {
        return 1.0 * m_num / m_den;
    }

    auto &operator+=(Rational const &other)
    {
        auto lcm_den = std::lcm(m_den, other.m_den);
        m_num = m_num * (lcm_den / m_den) + other.m_num * (lcm_den / other.m_den);
        m_den = lcm_den;
        reduce();
        return *this;
    }

    auto &operator-=(Rational const &other)
    {
        return *this += Rational(-other.m_num, other.m_den);
    }

    auto &operator*=(Rational const &other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    auto &operator/=(Rational const &other)
    {
        return *this *= Rational(other.m_den, other.m_num);
    }

    auto operator++(int) { auto tmp = *this; *this += 1; return tmp; }
    auto operator--(int) { auto tmp = *this; *this -= 1; return tmp; }

    auto &operator++() { *this += 1; return *this; }
    auto &operator--() { *this -= 1; return *this; }

    friend auto operator+(Rational lhs, Rational const &rhs) { return lhs += rhs; }
    friend auto operator-(Rational lhs, Rational const &rhs) { return lhs -= rhs; }
    friend auto operator*(Rational lhs, Rational const &rhs) { return lhs *= rhs; }
    friend auto operator/(Rational lhs, Rational const &rhs) { return lhs /= rhs; }

    friend std::strong_ordering operator<=>(Rational const &lhs, Rational const &rhs)
    {
        auto left  = lhs.m_num * rhs.m_den;
        auto right = rhs.m_num * lhs.m_den;

        if (left < right) return std::strong_ordering::less;
        if (left > right) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const &lhs, Rational const &rhs)
    {
        return (lhs <=> rhs) == std::strong_ordering::equal;
    }

    friend std::istream &operator>>(std::istream &stream, Rational &rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend std::ostream &operator<<(std::ostream &stream, Rational const &rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    void reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        auto g = std::gcd(m_num, m_den);
        m_num /= g;
        m_den /= g;
    }

    int m_num = 0;
    int m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

bool equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Rational x = 1, y(2, 1);

    assert(equal(static_cast<double>(x), 1.0));

    assert((x += y) == Rational(3, 1));
    assert((x -= y) == Rational(1, 1));
    assert((x *= y) == Rational(2, 1));
    assert((x /= y) == Rational(1, 1));

    assert((x++) == Rational(1, 1));
    assert((x--) == Rational(2, 1));
    assert((++y) == Rational(3, 1));
    assert((--y) == Rational(2, 1));

    assert((x + y) == Rational(3, 1));
    assert((x - y) == Rational(-1, 1));
    assert((x * y) == Rational(2, 1));
    assert((x / y) == Rational(1, 2));

    assert((x += 1) == Rational(2, 1));
    assert((x + 1) == Rational(3, 1));
    assert((1 + y) == Rational(3, 1));
    assert((1 + 1) == Rational(2, 1));

    Rational a(1, 2), b(2, 4), c(3, 4), d(-1, 2);

    assert(a == b);
    assert(a != c);
    assert(a <  c);
    assert(c >  a);
    assert(a <= b);
    assert(c >= a);
    assert(d < a);
    assert((d <=> a) == std::strong_ordering::less);

    std::stringstream ss("10/20");
    Rational z;
    ss >> z;
    assert(z == Rational(1, 2));

    std::stringstream ss_out;
    ss_out << z;
    assert(ss_out.str() == "1/2");

    return 0;
}
