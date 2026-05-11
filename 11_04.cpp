#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

using ResultVariant =
    std::variant<double, std::pair<double, double>, std::monostate>;

class Visitor {
public:
  void operator()(double root) const { std::cout << root << '\n'; }
  void operator()(const std::pair<double, double> &roots) const {
    std::cout << roots.first << ' ' << roots.second << '\n';
  }
  void operator()(std::monostate) const {
    std::cout << "Infinite number of solutions\n";
  }
};

auto solve(double a, double b, double c) -> std::optional<ResultVariant> {
  const double epsilon = 1e-12;

  if (std::abs(a) <= epsilon) {
    if (std::abs(b) <= epsilon) {
      if (std::abs(c) <= epsilon) {
        return std::monostate{};
      } else {
        return std::nullopt;
      }
    } else {
      double x = -c / b;
      return x;
    }
  }

  double discriminant = b * b - 4.0 * a * c;

  if (discriminant > epsilon) {
    double sqrt_d = std::sqrt(discriminant);
    double x1 = (-b - sqrt_d) / (2.0 * a);
    double x2 = (-b + sqrt_d) / (2.0 * a);

    if (x1 > x2)
      std::swap(x1, x2);
    return std::make_pair(x1, x2);
  } else if (std::abs(discriminant) <= epsilon) {
    double x = -b / (2.0 * a);
    return x;
  } else {
    return std::nullopt;
  }
}

int main() {
  std::cout << "--- Task 11.04 ---" << std::endl;
  std::cout << "Enter coefficients a, b, c: ";

  double a = 0.0, b = 0.0, c = 0.0;
  if (!(std::cin >> a >> b >> c)) {
    return 1;
  }

  auto result = solve(a, b, c);

  if (!result) {
    std::cout << "No solutions\n";
  } else {

    std::visit(Visitor{}, *result);
  }

  return 0;
}
