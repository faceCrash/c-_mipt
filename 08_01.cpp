#include <cassert>
#include <iostream>

class Entity_v1 {
public:
  Entity_v1(int value) : m_value(value) {}
  int get() const { return m_value; }

private:
  int m_value;
};

class Entity_v2 {
public:
  int m_value;
};

void test_reinterpret_cast_hack() {
  Entity_v1 e1(42);
  assert(e1.get() == 42);

  auto &e2 = reinterpret_cast<Entity_v2 &>(e1);
  e2.m_value = 100;

  assert(e1.get() == 100);
  std::cout << "reinterpret_cast hack successful!\n";
}

class SecureEntity {
public:
  SecureEntity(int value) : m_secret(value) {}
  int get_secret() const { return m_secret; }

private:
  int m_secret;
};

template <typename Tag> struct AccessResult {
  static inline typename Tag::type ptr;
};

template <typename Tag, typename Tag::type p> struct AccessRobber {
  AccessRobber() { AccessResult<Tag>::ptr = p; }
  static AccessRobber instance;
};

template <typename Tag, typename Tag::type p>
AccessRobber<Tag, p> AccessRobber<Tag, p>::instance;

struct SecureEntitySecretTag {
  using type = int SecureEntity::*;
};

template class AccessRobber<SecureEntitySecretTag, &SecureEntity::m_secret>;

void test_template_hack() {
  SecureEntity entity(42);
  assert(entity.get_secret() == 42);

  int SecureEntity::*stolen_ptr = AccessResult<SecureEntitySecretTag>::ptr;

  entity.*stolen_ptr = 999;

  assert(entity.get_secret() == 999);
  std::cout << "Template instantiation hack successful!\n";
}

// -------------------------------------------------------------------------------------------------

int main() {
  test_reinterpret_cast_hack();
  test_template_hack();
  return 0;
}