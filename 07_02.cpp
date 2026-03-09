#include <cassert>
#include <cmath>
#include <compare>
#include <exception>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <variant>
#include <vector>

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

class Exception : public std::exception {
public:
  explicit Exception(const std::string &message) : m_message(message) {}
  const char *what() const noexcept override { return m_message.c_str(); }

private:
  std::string m_message;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational : public addable<Rational>,
                 public subtractable<Rational>,
                 public multipliable<Rational>,
                 public dividable<Rational>,
                 public incrementable<Rational>,
                 public decrementable<Rational> {
public:
  Rational(int num = 0, int den = 1) : m_num(num), m_den(den) {
    if (den == 0) {
      throw Exception("Denominator cannot be zero");
    }
    reduce();
  }

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

void demonstrate_exceptions() {
  try {
    std::vector<int> v;
    v.reserve(-1);
  } catch (const std::bad_alloc &e) {
    std::cerr << "Caught std::bad_alloc: " << e.what()
              << " (Причина: нехватка памяти или запрос на выделение слишком "
                 "большого блока памяти).\n";
  }
  catch (const std::exception& e)
  {
	  std::cerr << "Caught  v.reserve(-1): " << e.what() << "\n";
  }
  /*
   * Because, your code was exiting here without (const std::exception& e)
   * 
   * And anyway,
   * For everything else, you need to add
   * catch (const std::exception& e)
   */
    
  try {
    std::variant<int, double> var = 42;
    std::get<double>(var);
  } catch (const std::bad_variant_access &e) {
    std::cerr << "Caught std::bad_variant_access: " << e.what()
        << " (Причина: обращение к неактивному типу варианта std::variant).\n";
  }

  try {
    std::optional<int> opt;
    opt.value();
  } catch (const std::bad_optional_access &e) {
    std::cerr
        << "Caught std::bad_optional_access: " << e.what()
        << " (Причина: попытка получить значение из пустого std::optional).\n";
  }

  try {
    std::vector<int> v;
    v.resize(v.max_size() + 1);
  } catch (const std::length_error &e) {
    std::cerr << "Caught std::length_error: " << e.what()
              << " (Причина: превышение максимального возможного размера "
                 "контейнера std::length_error).\n";
  }

  try {
    std::vector<int> v = {1, 2, 3};
    v.at(5);
  } catch (const std::out_of_range &e) {
    std::cerr << "Caught std::out_of_range: " << e.what()
              << " (Причина: обращение за пределы массива при использовании "
                 "безопасного метода типа at()).\n";
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  try {
    demonstrate_exceptions();

   Rational valid(1, 2);
   std::cerr << "Rational(1, 2) is valid\n";

   Rational invalid(1, 0);
   std::cerr << "Эта строка не напечатается\n";
  } catch (const Exception &e) {
    std::cerr << "Caught custom Exception: " << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "Caught std::exception: " << e.what() << '\n';
  } catch (...) {
    std::cerr << "Caught unknown exception\n";
  }

  return 0;
}

/*
 * 1. when compiled and executed, the code exited without printing anything after v.reserve(-1);
 * 2. catch (const std::exception& e) was not added in sequence for all the std_demonstration
 * 
 * the score is 8/10
 */
