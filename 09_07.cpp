#include <iostream>
#include <array>
#include <memory>
#include <bit>
#include <cstddef>
#include <boost/noncopyable.hpp>

/**
 * @brief Entity class using Fast Pimpl (Static Pimpl).
 * 
 * Performance Justification:
 * 1. Reduced Allocations: Implementation is stored within the Entity memory block, 
 *    eliminating the need for a separate 'new Implementation' call.
 * 2. Better Cache Locality: The Entity and its Implementation are contiguous in memory,
 *    reducing potential cache misses during access.
 * 3. Overhead: The Entity size is fixed at compile-time and must be large enough to 
 *    hold the Implementation. It also requires careful manual lifetime management.
 */
class Entity : private boost::noncopyable {
public:
    Entity();
    ~Entity();

    void test() const;

    // Const and non-const getters for Implementation
    class Implementation;
    
    Implementation* get();
    const Implementation* get() const;

private:
    // Storage for Implementation (16 bytes, aligned to max_align_t)
    alignas(std::max_align_t) std::array<std::byte, 16> m_storage;
};

// --- Implementation Part ---
class Entity::Implementation {
public:
    void do_test() const {
        std::cout << "Fast Pimpl: Implementation::do_test() called.\n";
    }
};

Entity::Entity() {
    // Static checks for size and alignment
    static_assert(sizeof(Implementation) <= sizeof(m_storage), 
                 "Implementation size exceeds storage size.");
    static_assert(alignof(Implementation) <= alignof(decltype(m_storage)), 
                 "Implementation alignment exceeds storage alignment.");

    // Placement new
    new (m_storage.data()) Implementation();
}

Entity::~Entity() {
    // Explicit destruction
    std::destroy_at(get());
}

Entity::Implementation* Entity::get() {
    // std::bit_cast for pointer conversion + std::launder for safety
    return std::launder(std::bit_cast<Implementation*>(m_storage.data()));
}

const Entity::Implementation* Entity::get() const {
    // std::bit_cast + std::launder for const pointer
    return std::launder(std::bit_cast<const Implementation*>(m_storage.data()));
}

void Entity::test() const {
    get()->do_test();
}

int main() {
    Entity e;
    e.test();
    return 0;
}
