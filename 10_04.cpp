#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>

using namespace boost::numeric::ublas;
typedef matrix<unsigned long long> Matrix;

Matrix multiply(const Matrix &A, const Matrix &B) {
  Matrix C(2, 2);
  C(0, 0) = A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0);
  C(0, 1) = A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1);
  C(1, 0) = A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0);
  C(1, 1) = A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1);
  return C;
}

Matrix power(Matrix A, unsigned int n) {
  Matrix res(2, 2);

  res(0, 0) = 1;
  res(0, 1) = 0;
  res(1, 0) = 0;
  res(1, 1) = 1;

  while (n > 0) {
    if (n % 2 == 1)
      res = multiply(res, A);
    A = multiply(A, A);
    n /= 2;
  }
  return res;
}

unsigned long long fibonacci(unsigned int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;

  Matrix M(2, 2);
  M(0, 0) = 1;
  M(0, 1) = 1;
  M(1, 0) = 1;
  M(1, 1) = 0;

  Matrix res = power(M, n - 1);
  return res(0, 0);
}

int main() {
  unsigned int n_values[] = {0, 1, 2, 3, 5, 10, 20, 50, 90};

  std::cout << "Task 10.04: Fibonacci with Matrix Exponentiation O(log N)"
            << std::endl;
  std::cout << "--------------------------------------------------------"
            << std::endl;

  for (unsigned int n : n_values) {
    std::cout << "F(" << n << ") = " << fibonacci(n) << std::endl;
  }

  std::cout << "\nAlgorithm Complexity Comparison:" << std::endl;
  std::cout
      << "1. Recursive: O(phi^N) - Exponential time, extremely slow for N > 40."
      << std::endl;
  std::cout << "2. Iterative: O(N) - Linear time, simple but slower than "
               "logarithmic for large N."
            << std::endl;
  std::cout << "3. Binet's Formula: O(1) - Constant time, but limited by "
               "floating-point precision."
            << std::endl;
  std::cout << "4. Matrix Exponentiation: O(log N) - Logarithmic time, optimal "
               "for very large N."
            << std::endl;

  return 0;
}
