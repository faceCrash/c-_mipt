#include <iostream>
#include <string>

struct SwordBehavior
{
    void attack() const { std::cout << "Swinging sword!\n"; }
};

struct MagicBehavior
{
    void attack() const { std::cout << "Casting fireball!\n"; }
};

struct BowBehavior
{
    void attack() const { std::cout << "Shooting arrow!\n"; }
};

template <typename Behavior>
class Entity : private Behavior
{
public:
    using Behavior::attack;

    Entity() = default;

    void info() const
    {
        std::cout << "Entity using behavior: " << typeid(Behavior).name() << "\n";
    }
};

int main()
{
    Entity<SwordBehavior> warrior;
    Entity<MagicBehavior> mage;
    Entity<BowBehavior> archer;

    warrior.info();
    warrior.attack();

    mage.info();
    mage.attack();

    archer.info();
    archer.attack();
}
