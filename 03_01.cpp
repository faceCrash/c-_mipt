#include <iostream>
#include <vector>
#include <algorithm> // для std::max и std::min

// Структура для представления прямоугольника
struct Rectangle {
    double x1, y1; // Координаты левого верхнего угла
    double x2, y2; // Координаты правого нижнего угла
};

// Функция для вычисления площади пересечения двух прямоугольников
double intersectionArea(const Rectangle& r1, const Rectangle& r2) {
    // Находим координаты пересечения
    double x_left = std::max(r1.x1, r2.x1);
    double y_top = std::max(r1.y1, r2.y1);
    double x_right = std::min(r1.x2, r2.x2);
    double y_bottom = std::min(r1.y2, r2.y2);

    // Проверка на отсутствие пересечения
    if (x_right < x_left || y_bottom < y_top) {
        return 0.0;
    }

    // Вычисляем площадь пересечения
    return (x_right - x_left) * (y_bottom - y_top);
}

// Функция для вычисления площади пересечения нескольких прямоугольников
double totalIntersectionArea(const std::vector<Rectangle>& rects) {
    if (rects.empty()) return 0.0;

    Rectangle intersect = rects[0];

    for (size_t i = 1; i < rects.size(); ++i) {
        intersect.x1 = std::max(intersect.x1, rects[i].x1);
        intersect.y1 = std::max(intersect.y1, rects[i].y1);
        intersect.x2 = std::min(intersect.x2, rects[i].x2);
        intersect.y2 = std::min(intersect.y2, rects[i].y2);

        if (intersect.x2 < intersect.x1 || intersect.y2 < intersect.y1) {
            return 0.0; // Нет пересечения
        }
    }

    return (intersect.x2 - intersect.x1) * (intersect.y2 - intersect.y1);
}

// Функция для вычисления минимального ограничивающего прямоугольника
Rectangle boundingRectangle(const std::vector<Rectangle>& rects) {
    Rectangle bound = rects[0];

    for (size_t i = 1; i < rects.size(); ++i) {
        bound.x1 = std::min(bound.x1, rects[i].x1);
        bound.y1 = std::min(bound.y1, rects[i].y1);
        bound.x2 = std::max(bound.x2, rects[i].x2);
        bound.y2 = std::max(bound.y2, rects[i].y2);
    }

    return bound;
}

// Пример использования
int main() {
    std::vector<Rectangle> rects = {
        {0, 0, 4, 4},
        {2, 2, 6, 6},
        {3, 1, 5, 5}
    };

    std::cout << "Площадь пересечения: " << totalIntersectionArea(rects) << "\n";

    Rectangle bound = boundingRectangle(rects);
    std::cout << "Минимальный ограничивающий прямоугольник: "
              << "x1=" << bound.x1 << ", y1=" << bound.y1
              << ", x2=" << bound.x2 << ", y2=" << bound.y2 << "\n";

    return 0;
}
