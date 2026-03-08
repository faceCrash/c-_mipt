#include <iostream>
#include <string>
#include <cassert>

class Entity
{
public:
    virtual ~Entity() = default;
    virtual std::string name() const = 0;
    virtual int value() const = 0;
};

class BaseEntity : public virtual Entity
{
public:
    BaseEntity(std::string n, int v)
        : m_name(std::move(n)), m_value(v) {}

    std::string name() const override { return m_name; }
    int value() const override { return m_value; }

private:
    std::string m_name;
    int m_value = 0;
};

template <typename T>
class Decorator : public virtual Entity, public T
{
public:
    template <typename... Args>
    Decorator(Args&&... args)
        : T(std::forward<Args>(args)...) {}

    std::string name() const override
    {
        return "Decorated(" + T::name() + ")";
    }

    int value() const override
    {
        return T::value() + 10;
    }
};

int main()
{
    BaseEntity entity("Test", 42);

    Decorator<BaseEntity> decorated(entity.name(), entity.value());

    assert(entity.name() == "Test");
    assert(entity.value() == 42);

    assert(decorated.name() == "Decorated(Test)");
    assert(decorated.value() == 52);

    std::cout << "Entity: " << entity.name() << " = " << entity.value() << "\n";
    std::cout << "Decorated: " << decorated.name() << " = " << decorated.value() << "\n";
}