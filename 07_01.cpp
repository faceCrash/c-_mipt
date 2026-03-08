#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

using ResultVariant =
    std::variant<double, std::pair<double, double>, std::monostate>;

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
    // keep roots sorted just in case
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
  double a = 0.0, b = 0.0, c = 0.0;
  if (!(std::cin >> a >> b >> c)) {
    return 1;
  }

  auto result = solve(a, b, c);

  if (!result) {
    std::cout << "No solutions\n";
  } else {
    auto val = result.value();
    if (std::holds_alternative<double>(val)) {
      std::cout << std::get<double>(val) << '\n';
    } else if (std::holds_alternative<std::pair<double, double>>(val)) {
      auto roots = std::get<std::pair<double, double>>(val);
      std::cout << roots.first << ' ' << roots.second << '\n';
    } else if (std::holds_alternative<std::monostate>(val)) {
      std::cout << "Infinite number of solutions\n";
    }
  }

  return 0;
}
