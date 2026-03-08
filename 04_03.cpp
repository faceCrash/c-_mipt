#include <iostream>
#include <vector>

// ================== Функция добавления всех int ==================
template <typename Container, typename... Args>
void add_all(Container& c, Args&&... args) {
    // Используем fold-подобный трюк через initializer_list и оператор запятая
    (void)std::initializer_list<int>{ (c.push_back(args), 0)... };
}

// ================== Тестирование ==================
int main() {
    std::vector<int> v;

    add_all(v, 1, 2, 3, 4, 5);

    std::cout << "Вектор содержит: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << "\n";

    // Можно добавить ещё элементы
    add_all(v, 10, 20, 30);

    std::cout << "После добавления ещё элементов: ";
    for (auto x : v) std::cout << x << " ";
    std::cout << "\n";

    return 0;
}
