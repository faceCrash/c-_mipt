#include <iostream>
#include <memory>

struct Entity {
  int x = 0, y = 0;
  ~Entity() { std::cout << "Entity destroyed\n"; }
};

class Builder {
public:
  virtual ~Builder() = default;

  std::unique_ptr<Entity> make_entity() {
    m_entity = std::make_unique<Entity>();
    set_x();
    set_y();
    return std::move(m_entity);
  }

  virtual void set_x() const = 0;
  virtual void set_y() const = 0;

protected:
  std::unique_ptr<Entity> m_entity;
};

class Builder_Client : public Builder {
public:
  void set_x() const override { m_entity->x = 1; }
  void set_y() const override { m_entity->y = 1; }
};

class Builder_Server : public Builder {
public:
  void set_x() const override { m_entity->x = 2; }
  void set_y() const override { m_entity->y = 2; }
};

int main() {
  auto builder = std::make_unique<Builder_Client>();
  auto entity = builder->make_entity();
  std::cout << "Entity: " << entity->x << ", " << entity->y << "\n";
}
