#include <iostream>
#include <cmath> // Для std::sqrt

// Константа числа π для C++14
constexpr double PI = 3.14159265358979323846;

// Класс треугольника
class Triangle {
public:
    double a, b, c; // Стороны треугольника

    Triangle(double a_, double b_, double c_) : a(a_), b(b_), c(c_) {}

    double perimeter() const {
        return a + b + c;
    }

    double area() const {
        double s = perimeter() / 2.0;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }
};

// Класс квадрата
class Square {
public:
    double side;

    Square(double s) : side(s) {}

    double perimeter() const {
        return 4 * side;
    }

    double area() const {
        return side * side;
    }
};

// Класс окружности
class Circle {
public:
    double radius;

    Circle(double r) : radius(r) {}

    double perimeter() const {
        return 2.0 * PI * radius; // Используем нашу константу PI
    }

    double area() const {
        return PI * radius * radius; // Используем нашу константу PI
    }
};

// Пример использования
int main() {
    Triangle t(3, 4, 5);
    Square s(4);
    Circle c(2);

    std::cout << "Треугольник: периметр = " << t.perimeter() << ", площадь = " << t.area() << "\n";
    std::cout << "Квадрат: периметр = " << s.perimeter() << ", площадь = " << s.area() << "\n";
    std::cout << "Окружность: периметр = " << c.perimeter() << ", площадь = " << c.area() << "\n";

    return 0;
}
