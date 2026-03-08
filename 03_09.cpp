#include <iostream>
#include <cstdint>
#include <array>
#include <stdexcept>

class IPv4 {
private:
    std::array<std::uint8_t, 4> bytes;

    // Вспомогательная функция: преобразование IP в 32-битное число
    std::uint32_t to_uint() const {
        return (static_cast<std::uint32_t>(bytes[0]) << 24) |
               (static_cast<std::uint32_t>(bytes[1]) << 16) |
               (static_cast<std::uint32_t>(bytes[2]) << 8)  |
               (static_cast<std::uint32_t>(bytes[3]));
    }

    // Обратное преобразование
    void from_uint(std::uint32_t value) {
        bytes[0] = static_cast<std::uint8_t>((value >> 24) & 0xFF);
        bytes[1] = static_cast<std::uint8_t>((value >> 16) & 0xFF);
        bytes[2] = static_cast<std::uint8_t>((value >> 8) & 0xFF);
        bytes[3] = static_cast<std::uint8_t>(value & 0xFF);
    }

public:
    IPv4() : bytes{0, 0, 0, 0} {}

    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
        : bytes{a, b, c, d} {}

    // Префиксный инкремент
    IPv4& operator++() {
        from_uint(to_uint() + 1);
        return *this;
    }

    // Постфиксный инкремент
    IPv4 operator++(int) {
        IPv4 temp = *this;
        ++(*this);
        return temp;
    }

    // Префиксный декремент
    IPv4& operator--() {
        from_uint(to_uint() - 1);
        return *this;
    }

    // Постфиксный декремент
    IPv4 operator--(int) {
        IPv4 temp = *this;
        --(*this);
        return temp;
    }

    // Операторы сравнения
    friend bool operator==(const IPv4& lhs, const IPv4& rhs) {
        return lhs.bytes == rhs.bytes;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs) {
        return lhs.to_uint() < rhs.to_uint();
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs < rhs);
    }

    // Ввод
    friend std::istream& operator>>(std::istream& in, IPv4& ip) {
        int a, b, c, d;
        char dot1, dot2, dot3;
        in >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;
        if (!in || dot1 != '.' || dot2 != '.' || dot3 != '.')
            in.setstate(std::ios::failbit);
        else if (a < 0 || a > 255 || b < 0 || b > 255 ||
                 c < 0 || c > 255 || d < 0 || d > 255)
            in.setstate(std::ios::failbit);
        else
            ip = IPv4(static_cast<std::uint8_t>(a),
                      static_cast<std::uint8_t>(b),
                      static_cast<std::uint8_t>(c),
                      static_cast<std::uint8_t>(d));
        return in;
    }

    // Вывод
    friend std::ostream& operator<<(std::ostream& out, const IPv4& ip) {
        out << static_cast<int>(ip.bytes[0]) << '.'
            << static_cast<int>(ip.bytes[1]) << '.'
            << static_cast<int>(ip.bytes[2]) << '.'
            << static_cast<int>(ip.bytes[3]);
        return out;
    }
};

// -------------------------- MAIN ------------------------------

int main() {
    IPv4 ip1(192, 168, 0, 1);
    IPv4 ip2;

    std::cout << "Enter IP address (format: a.b.c.d): ";
    std::cin >> ip2;

    if (!std::cin) {
        std::cerr << "Invalid IP format!\n";
        return 1;
    }

    std::cout << "You entered: " << ip2 << "\n";

    std::cout << "ip1 = " << ip1 << "\n";
    std::cout << "++ip1 = " << ++ip1 << "\n";
    std::cout << "ip1++ = " << ip1++ << "\n";
    std::cout << "After ip1++: " << ip1 << "\n";

    std::cout << "--ip1 = " << --ip1 << "\n";
    std::cout << "ip1-- = " << ip1-- << "\n";
    std::cout << "After ip1--: " << ip1 << "\n";

    std::cout << std::boolalpha;
    std::cout << "ip1 == ip2: " << (ip1 == ip2) << "\n";
    std::cout << "ip1 < ip2:  " << (ip1 < ip2) << "\n";

    return 0;
}
