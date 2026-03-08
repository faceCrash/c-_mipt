#include <iostream>

// Структура узла односвязного списка
struct Node {
    int value;     // Значение узла
    Node* next;    // Указатель на следующий узел

    Node(int v) : value(v), next(nullptr) {} // Конструктор
};

// Класс односвязного списка
class List {
private:
    Node* head; // Указатель на головной узел
    Node* tail; // Указатель на хвостовой узел

public:
    // Конструктор списка
    List() : head(nullptr), tail(nullptr) {}

    // Деструктор: освобождает всю память
    ~List() {
        while (!empty()) {
            pop_front();
        }
    }

    // Проверка, пуст ли список
    bool empty() const {
        return head == nullptr;
    }

    // Вывод значений всех узлов
    void show() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << "\n";
    }

    // Добавление узла в начало
    void push_front(int v) {
        Node* newNode = new Node(v);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) { // Если список был пуст
            tail = newNode;
        }
    }

    // Добавление узла в конец
    void push_back(int v) {
        Node* newNode = new Node(v);
        if (tail != nullptr) {
            tail->next = newNode;
        }
        tail = newNode;
        if (head == nullptr) { // Если список был пуст
            head = newNode;
        }
    }

    // Удаление узла с начала
    void pop_front() {
        if (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
            if (head == nullptr) { // Если список стал пустым
                tail = nullptr;
            }
        }
    }

    // Удаление узла с конца
    void pop_back() {
        if (head == nullptr) return; // Пустой список
        if (head == tail) {          // Один узел
            delete head;
            head = tail = nullptr;
            return;
        }
        // Проходим до предпоследнего узла
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    // Получение значения среднего узла
    int get() const {
        if (head == nullptr) return 0; // Если список пустой
        Node* slow = head;
        Node* fast = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->value;
    }
};

// Пример использования
int main() {
    List lst;

    lst.push_back(10);
    lst.push_back(20);
    lst.push_back(30);
    lst.push_front(5);

    std::cout << "Список: ";
    lst.show();

    std::cout << "Средний узел: " << lst.get() << "\n";

    lst.pop_front();
    std::cout << "После pop_front: ";
    lst.show();

    lst.pop_back();
    std::cout << "После pop_back: ";
    lst.show();

    return 0;
}