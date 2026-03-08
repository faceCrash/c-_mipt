#include <cassert>
#include <cmath>
#include <compare>
#include <numeric>

///////////////////////////////////////////////////////////////////////////////////////////////

template <class T> struct addable {
  friend T operator+(T lhs, const T &rhs) { return lhs += rhs; }
};

template <class T> struct subtractable {
  friend T operator-(T lhs, const T &rhs) { return lhs -= rhs; }
};

template <class T> struct multipliable {
  friend T operator*(T lhs, const T &rhs) { return lhs *= rhs; }
};

template <class T> struct dividable {
  friend T operator/(T lhs, const T &rhs) { return lhs /= rhs; }
};

template <class T> struct incrementable {
  friend T &operator++(T &lhs) { return lhs += 1; }
  friend T operator++(T &lhs, int) {
    T tmp(lhs);
    ++lhs;
    return tmp;
  }
};

template <class T> struct decrementable {
  friend T &operator--(T &lhs) { return lhs -= 1; }
  friend T operator--(T &lhs, int) {
    T tmp(lhs);
    --lhs;
    return tmp;
  }
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational : public addable<Rational>,
                 public subtractable<Rational>,
                 public multipliable<Rational>,
                 public dividable<Rational>,
                 public incrementable<Rational>,
                 public decrementable<Rational> {
public:
  Rational(int num = 0, int den = 1) : m_num(num), m_den(den) { reduce(); }

  explicit operator double() const { return 1.0 * m_num / m_den; }

  auto &operator+=(Rational const &other) {
    auto lcm_den = std::lcm(m_den, other.m_den);
    m_num = m_num * (lcm_den / m_den) + other.m_num * (lcm_den / other.m_den);
    m_den = lcm_den;
    reduce();
    return *this;
  }

  auto &operator-=(Rational const &other) {
    return *this += Rational(-other.m_num, other.m_den);
  }

  auto &operator*=(Rational const &other) {
    m_num *= other.m_num;
    m_den *= other.m_den;
    reduce();
    return *this;
  }

  auto &operator/=(Rational const &other) {
    return *this *= Rational(other.m_den, other.m_num);
  }

  friend std::strong_ordering operator<=>(Rational const &lhs,
                                          Rational const &rhs) {
    auto left = lhs.m_num * rhs.m_den;
    auto right = rhs.m_num * lhs.m_den;

    if (left < right)
      return std::strong_ordering::less;
    if (left > right)
      return std::strong_ordering::greater;
    return std::strong_ordering::equal;
  }

  friend bool operator==(Rational const &lhs, Rational const &rhs) {
    return (lhs <=> rhs) == std::strong_ordering::equal;
  }

private:
  void reduce() {
    if (m_den < 0) {
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

bool equal(double x, double y, double epsilon = 1e-6) {
  return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void test_addition() {
  Rational x(1, 2);
  Rational y(1, 4);
  Rational z = x + y;
  assert(z == Rational(3, 4));
}

void test_subtraction() {
  Rational x(1, 2);
  Rational y(1, 4);
  Rational z = x - y;
  assert(z == Rational(1, 4));
}

void test_multiplication_and_division() {
  Rational x(1, 2);
  Rational y(1, 4);
  Rational z = x * y;
  assert(z == Rational(1, 8));

  Rational w = x / y;
  assert(w == Rational(2, 1));
}

void test_increments() {
  Rational x(1, 2);
  Rational y = x++;
  assert(y == Rational(1, 2));
  assert(x == Rational(3, 2));

  Rational z = ++x;
  assert(z == Rational(5, 2));
  assert(x == Rational(5, 2));
}

void test_decrements() {
  Rational x(3, 2);
  Rational y = x--;
  assert(y == Rational(3, 2));
  assert(x == Rational(1, 2));

  Rational z = --x;
  assert(z == Rational(-1, 2));
  assert(x == Rational(-1, 2));
}

void test_comparison() {
  Rational a(1, 2), b(2, 4), c(3, 4), d(-1, 2);
  assert(a == b);
  assert(a != c);
  assert(a < c);
  assert(c > a);
  assert(a <= b);
  assert(c >= a);
  assert(d < a);
}

int main() {
  test_addition();
  test_subtraction();
  test_multiplication_and_division();
  test_increments();
  test_decrements();
  test_comparison();

  return 0;
}
