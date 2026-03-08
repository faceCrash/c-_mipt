// 02_04_armstrong.cpp
// Поиск всех трёхзначных чисел Армстронга.
// Число Армстронга (для трёхзначных чисел) — такое число abc, что
// a^3 + b^3 + c^3 = abc.
// Подход: тройной вложенный цикл по цифрам a (1..9), b (0..9), c (0..9).

#include <iostream>

int main() {
    for (int a = 1; a <= 9; ++a) {      // сотни: 1..9 (число трёхзначное)
        for (int b = 0; b <= 9; ++b) {  // десятки: 0..9
            for (int c = 0; c <= 9; ++c) { // единицы: 0..9
                int number = a * 100 + b * 10 + c;
                int sum_cubes = a*a*a + b*b*b + c*c*c; // без std::pow
                if (sum_cubes == number) {
                    std::cout << number << '\n';
                }
            }
        }
    }
    return 0;
}
