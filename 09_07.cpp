#include <array>
#include <bit>
#include <boost/noncopyable.hpp>
#include <cstddef>
#include <iostream>
#include <memory>

class Entity : private boost::noncopyable {
public:
  Entity();
  ~Entity();

  void test() const;

  class Implementation;

  Implementation *get();
  const Implementation *get() const;

private:
  alignas(std::max_align_t) std::array<std::byte, 16> m_storage;
};

class Entity::Implementation {
public:
  void do_test() const {
    std::cout << "Fast Pimpl: Implementation::do_test() called.\n";
  }
};

Entity::Entity() {

  static_assert(sizeof(Implementation) <= sizeof(m_storage),
                "Implementation size exceeds storage size.");
  static_assert(alignof(Implementation) <= alignof(decltype(m_storage)),
                "Implementation alignment exceeds storage alignment.");

  new (m_storage.data()) Implementation();
}

Entity::~Entity() { std::destroy_at(get()); }

Entity::Implementation *Entity::get() {

  return std::launder(std::bit_cast<Implementation *>(m_storage.data()));
}

const Entity::Implementation *Entity::get() const {

  return std::launder(std::bit_cast<const Implementation *>(m_storage.data()));
}

void Entity::test() const { get()->do_test(); }

int main() {
  Entity e;
  e.test();
  return 0;
}
