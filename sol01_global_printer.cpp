#include <iostream>
#include <sstream>   // нужен для std::ostringstream
#include <string>
#include <cassert>

// Класс-«принтер»: конструктор печатает строку
class Printer {
public:
    Printer() {
        std::cout << "Solution A: Hello, world!\n";
    }
};

// Глобальный объект, создающийся до main
[[maybe_unused]] Printer printer_instance;

// ----------- Тесты -----------
// Проверка работы конструктора через перенаправление std::cout
void run_tests() {
    std::ostringstream buffer; // временный буфер для вывода
    std::streambuf* old_buf = std::cout.rdbuf(buffer.rdbuf()); // перенаправили cout

    {
        Printer test_printer; // создаём временный объект
    }

    std::cout.rdbuf(old_buf); // вернули cout обратно

    std::string output = buffer.str();
    assert(output == "Solution A: Hello, world!\n"); // проверка результата
}

// Глобальный запуск тестов (до main)
[[maybe_unused]] int run_all_tests = []() {
    run_tests();
    return 0;
}();

int main() {
    // Единственная инструкция в main
    return 0;
}
