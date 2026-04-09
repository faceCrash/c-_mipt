#include <iostream>
#include <memory>
#include <iterator>

template <typename T>
class List {
private:
    struct Node {
        T x = T();
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev; // weak_ptr for backward link

        Node(T val) : x(val) {}
        ~Node() { std::cout << "[DEBUG] Node " << x << " destroyed\n"; }
    };

public:
    class Iterator {
    public:
        // Category updated to bidirectional
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(std::shared_ptr<Node> node = nullptr) : m_node(node) {}

        // Prefix increment
        auto& operator++() {
            if (m_node) m_node = m_node->next;
            return *this;
        }

        // Postfix increment
        auto operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        // Prefix decrement
        auto& operator--() {
            if (m_node) m_node = m_node->prev.lock();
            return *this;
        }

        // Postfix decrement
        auto operator--(int) {
            auto tmp = *this;
            --(*this);
            return tmp;
        }

        auto& operator*() const { return m_node->x; }
        auto operator->() const { return &m_node->x; }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_node == rhs.m_node;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return !(lhs == rhs);
        }

    private:
        std::shared_ptr<Node> m_node;
    };

    auto begin() const { return Iterator(m_head); }
    auto end() const { return Iterator(nullptr); }

    void push_back(T x) {
        auto node = std::make_shared<Node>(x);
        if (m_tail) {
            m_tail->next = node;
            node->prev = m_tail;
            m_tail = node;
        } else {
            m_head = m_tail = node;
        }
    }

private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail; // Optimized push_back
};

int main() {
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    std::cout << "Forward: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Demonstrate bidirectional iteration
    std::cout << "Backward: ";
    auto it = list.begin();
    std::advance(it, 2); // it points to 3
    std::cout << *it << " ";
    --it;
    std::cout << *it << " ";
    --it;
    std::cout << *it << "\n";

    return 0;
}
