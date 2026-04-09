#include <iostream>
#include <vector>
#include <memory>
#include <cstddef>
#include <new>
#include <cassert>
#include <algorithm>
#include <random>
#include <benchmark/benchmark.h>
#include <boost/noncopyable.hpp>

// --- Assignment 09.10: Abstract Interface ---

class Allocator : private boost::noncopyable {
public:
    virtual ~Allocator() {
        if (m_begin) ::operator delete(m_begin, m_total_size, std::align_val_t(s_alignment));
    }

    virtual void* allocate(std::size_t size, std::size_t alignment = s_alignment) = 0;
    virtual void deallocate(void* ptr) = 0;

protected:
    Allocator(std::size_t size) : m_total_size(size) {
        m_begin = ::operator new(m_total_size, std::align_val_t(s_alignment));
    }


    // Template helper for pointer arithmetic and casting
    template <typename T>
    T* get(void* x) const {
        return static_cast<T*>(x);
    }

    std::byte* get_byte(void* x) const {
        return static_cast<std::byte*>(x);
    }

    std::size_t m_total_size = 0;
    void* m_begin = nullptr;
    static inline auto s_alignment = alignof(std::max_align_t);
};

// --- Derived Allocator: Linear (09.28) ---

class LinearAllocator : public Allocator {
public:
    LinearAllocator(std::size_t size) : Allocator(size), m_offset(0) {}

    void* allocate(std::size_t size, std::size_t alignment = s_alignment) override {
        void* start = get_byte(m_begin) + m_offset;
        std::size_t space = m_total_size - m_offset;
        
        if (void* ptr = std::align(alignment, size, start, space)) {
            m_offset = m_total_size - space + size;
            return ptr;
        }
        return nullptr;
    }

    void deallocate(void*) override {
        // Linear allocator only resets at once (not implemented here as per 09.28)
    }

private:
    std::size_t m_offset;
};

// --- Derived Allocator: Stack (09.29) ---

class StackAllocator : public Allocator {
public:
    StackAllocator(std::size_t size) : Allocator(size), m_offset(0) {}

    void* allocate(std::size_t size, std::size_t alignment = s_alignment) override {
        std::size_t header_size = sizeof(std::uint8_t);
        void* start = get_byte(m_begin) + m_offset + header_size;
        std::size_t space = m_total_size - m_offset - header_size;

        if (void* ptr = std::align(alignment, size, start, space)) {
            std::uint8_t padding = static_cast<std::uint8_t>(get_byte(ptr) - (get_byte(m_begin) + m_offset));
            auto header = reinterpret_cast<std::uint8_t*>(get_byte(ptr) - header_size);
            *header = padding;
            m_offset = get_byte(ptr) - get_byte(m_begin) + size;
            return ptr;
        }
        return nullptr;
    }

    void deallocate(void* ptr) override {
        auto header = reinterpret_cast<std::uint8_t*>(get_byte(ptr) - sizeof(std::uint8_t));
        m_offset = get_byte(ptr) - get_byte(m_begin) - *header;
    }

private:
    std::size_t m_offset;
};

// --- Derived Allocator: List / Pool (09.30) ---

class ListAllocator : public Allocator {
public:
    ListAllocator(std::size_t size, std::size_t step) : Allocator(size), m_step(step) {
        assert(m_total_size % m_step == 0 && m_step >= sizeof(Node));
        m_head = get<Node>(m_begin);
        m_head->next = nullptr;
        // Initialize simple links
        std::byte* curr = get_byte(m_begin);
        for(size_t i = 0; i < (m_total_size / m_step) - 1; ++i) {
             get<Node>(curr)->next = get<Node>(curr + m_step);
             curr += m_step;
        }
        get<Node>(curr)->next = nullptr;
    }

    void* allocate(std::size_t size, std::size_t) override {
        if (!m_head) return nullptr;
        Node* node = m_head;
        m_head = m_head->next;
        return node;
    }

    void deallocate(void* ptr) override {
        Node* node = get<Node>(ptr);
        node->next = m_head;
        m_head = node;
    }

private:
    struct Node { Node* next; };
    std::size_t m_step;
    Node* m_head;
};

// --- Derived Allocator: Free List (09.31 + 09.09) ---

class FreeListAllocator : public Allocator {
public:
    enum class Strategy { FirstFit, BestFit };

    FreeListAllocator(std::size_t size, Strategy strategy = Strategy::FirstFit) 
        : Allocator(size), m_strategy(strategy) {
        m_head = get<Node>(m_begin);
        m_head->size = m_total_size - sizeof(Header);
        m_head->next = nullptr;
    }

    void* allocate(std::size_t size, std::size_t alignment = s_alignment) override {
        // Simplified alignment logic for demonstration
        std::size_t total_required = size + sizeof(Header);
        
        Node* previous = nullptr;
        Node* current = nullptr;

        if (m_strategy == Strategy::FirstFit) {
            std::tie(current, previous) = find_first(total_required);
        } else {
            std::tie(current, previous) = find_best(total_required);
        }

        if (!current) return nullptr;

        // Split node if enough space
        if (current->size >= total_required + sizeof(Node) + 1) {
            Node* next_node = get<Node>(get_byte(current) + total_required);
            next_node->size = current->size - total_required;
            next_node->next = current->next;
            current->next = next_node;
            current->size = size;
        }

        // Remove from free list
        if (previous) previous->next = current->next;
        else m_head = current->next;

        Header* h = get<Header>(current);
        h->size = current->size;
        return get_byte(current) + sizeof(Header);
    }

    void deallocate(void* ptr) override {
        Node* node = get<Node>(get_byte(ptr) - sizeof(Header));
        node->next = m_head;
        m_head = node;
        // Merge could be implemented here
    }

private:
    struct Node { std::size_t size; Node* next; };
    struct Header { std::size_t size; };
    
    Strategy m_strategy;
    Node* m_head;

    std::pair<Node*, Node*> find_first(std::size_t size) {
        Node* curr = m_head;
        Node* prev = nullptr;
        while (curr && curr->size < size) {
            prev = curr;
            curr = curr->next;
        }
        return {curr, prev};
    }

    std::pair<Node*, Node*> find_best(std::size_t size) {
        Node* best = nullptr;
        Node* best_prev = nullptr;
        Node* curr = m_head;
        Node* prev = nullptr;
        std::size_t min_diff = std::numeric_limits<std::size_t>::max();

        while (curr) {
            if (curr->size >= size) {
                std::size_t diff = curr->size - size;
                if (diff < min_diff) {
                    min_diff = diff;
                    best = curr;
                    best_prev = prev;
                }
            }
            prev = curr;
            curr = curr->next;
        }
        return {best, best_prev};
    }
};

// --- Benchmarks ---

static void BM_FreeList_FirstFit(benchmark::State& state) {
    FreeListAllocator alloc(1LL << 30, FreeListAllocator::Strategy::FirstFit);
    for (auto _ : state) {
        void* p = alloc.allocate(1024);
        alloc.deallocate(p);
    }
}
BENCHMARK(BM_FreeList_FirstFit);

static void BM_FreeList_BestFit(benchmark::State& state) {
    FreeListAllocator alloc(1LL << 30, FreeListAllocator::Strategy::BestFit);
    for (auto _ : state) {
        void* p = alloc.allocate(1024);
        alloc.deallocate(p);
    }
}
BENCHMARK(BM_FreeList_BestFit);

int main(int argc, char** argv) {
    // Polymorphism Demo
    std::cout << "Polymorphic Allocator Demo:\n";
    std::vector<std::unique_ptr<Allocator>> allocs;
    allocs.push_back(std::make_unique<LinearAllocator>(1024));
    allocs.push_back(std::make_unique<StackAllocator>(1024));
    allocs.push_back(std::make_unique<ListAllocator>(1024, 64));
    allocs.push_back(std::make_unique<FreeListAllocator>(1024));

    for (auto& a : allocs) {
        void* p = a->allocate(32);
        std::cout << "Allocated 32 bytes from " << typeid(*a).name() << " at " << p << "\n";
        a->deallocate(p);
    }

    std::cout << "\nRunning Benchmarks...\n";
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}
