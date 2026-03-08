#include <iostream>
#include <cmath>

constexpr double PI = 3.14159265358979323846;

class Shape_v1 {
public:
    virtual ~Shape_v1() = default;
    virtual double perimeter() const = 0;
};

class Shape_v2 {
public:
    virtual ~Shape_v2() = default;
    virtual double area() const = 0;
};

class Adapter_v1 : public virtual Shape_v1 {
public:
    virtual double perimeter_v1() const = 0;
    double perimeter() const final { return perimeter_v1(); }
};

class Adapter_v2 : public virtual Shape_v2 {
public:
    virtual double area_v2() const = 0;
    double area() const final { return area_v2(); }
};

class Triangle final : public Adapter_v1, public Adapter_v2 {
public:
    Triangle(double a_, double b_, double c_) : a(a_), b(b_), c(c_) {}

    double perimeter_v1() const override {
        return a + b + c;
    }

    double area_v2() const override {
        double s = perimeter_v1() / 2.0;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

private:
    double a, b, c;
};

class Square final : public Adapter_v1, public Adapter_v2 {
public:
    explicit Square(double s_) : s(s_) {}

    double perimeter_v1() const override {
        return 4 * s;
    }

    double area_v2() const override {
        return s * s;
    }

private:
    double s;
};

class Circle final : public Adapter_v1, public Adapter_v2 {
public:
    explicit Circle(double r_) : r(r_) {}

    double perimeter_v1() const override {
        return 2 * PI * r;
    }

    double area_v2() const override {
        return PI * r * r;
    }

private:
    double r;
};

int main() {
    Triangle t(3, 4, 5);
    Square s(4);
    Circle c(2);

    Shape_v1* p1 = &t;
    Shape_v2* p2 = &t;

    std::cout << "Triangle: perimeter = " << p1->perimeter()
              << ", area = " << p2->area() << "\n";

    p1 = &s;
    p2 = &s;
    std::cout << "Square: perimeter = " << p1->perimeter()
              << ", area = " << p2->area() << "\n";

    p1 = &c;
    p2 = &c;
    std::cout << "Circle: perimeter = " << p1->perimeter()
              << ", area = " << p2->area() << "\n";
}
