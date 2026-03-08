#include <iostream>

// Класс Entity с приватными функциями
class Entity {
private:
    int value;

    // Приватная функция №1
    int doubleValue() const {
        return value * 2;
    }

    // Приватная функция №2
    int tripleValue() const {
        return value * 3;
    }

    // Паттерн Attorney-Client: объявляем дружбу тестировщикам
    friend class DoubleValueTester;
    friend class TripleValueTester;

public:
    Entity(int v) : value(v) {}
};

// Класс-тестировщик для функции doubleValue
class DoubleValueTester {
public:
    static int test(const Entity& e) {
        return e.doubleValue();
    }
};

// Класс-тестировщик для функции tripleValue
class TripleValueTester {
public:
    static int test(const Entity& e) {
        return e.tripleValue();
    }
};

// Пример использования и тестирования
int main() {
    Entity e(5);

    std::cout << "Тест doubleValue: " << DoubleValueTester::test(e) << "\n"; // 10
    std::cout << "Тест tripleValue: " << TripleValueTester::test(e) << "\n"; // 15

    return 0;
}
