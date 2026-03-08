#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cassert>

class Character
{
public:
    std::string name;
    int health = 100;
    int attackPower = 10;

    void info() const
    {
        std::cout << "Character: " << name 
                  << " [HP=" << health 
                  << ", ATK=" << attackPower << "]\n";
    }
};

class CharacterBuilder
{
public:
    CharacterBuilder& setName(const std::string& n) { m_character.name = n; return *this; }
    CharacterBuilder& setHealth(int h) { m_character.health = h; return *this; }
    CharacterBuilder& setAttack(int a) { m_character.attackPower = a; return *this; }

    Character get() { return m_character; }

private:
    Character m_character;
};

class GameUnit
{
public:
    virtual ~GameUnit() = default;
    virtual void info(int indent = 0) const = 0;
    virtual void attack() = 0;
};

using GameUnitPtr = std::shared_ptr<GameUnit>;

class SingleUnit : public GameUnit
{
public:
    SingleUnit(Character c) : m_character(std::move(c)) {}

    void info(int indent = 0) const override
    {
        std::cout << std::string(indent, ' ');
        m_character.info();
    }

    void attack() override
    {
        std::cout << m_character.name << " attacks for " << m_character.attackPower << " damage!\n";
    }

private:
    Character m_character;
};

class UnitGroup : public GameUnit
{
public:
    void addUnit(GameUnitPtr unit) { m_units.push_back(unit); }

    void info(int indent = 0) const override
    {
        std::cout << std::string(indent, ' ') << "UnitGroup:\n";
        for (auto& u : m_units) u->info(indent + 2);
    }

    void attack() override
    {
        for (auto& u : m_units) u->attack();
    }

private:
    std::vector<GameUnitPtr> m_units;
};

class AttackBehavior
{
public:
    virtual ~AttackBehavior() = default;

    void performAttack()
    {
        prepare();
        attack();
        finish();
    }

protected:
    virtual void prepare() { std::cout << "Preparing attack...\n"; }
    virtual void attack() = 0;
    virtual void finish() { std::cout << "Attack finished.\n"; }
};

class SwordAttack : public AttackBehavior
{
protected:
    void attack() override { std::cout << "Swinging sword!\n"; }
};

class MagicAttack : public AttackBehavior
{
protected:
    void attack() override { std::cout << "Casting fireball!\n"; }
};

int main()
{
    CharacterBuilder builder;
    auto hero = builder.setName("Hero").setHealth(150).setAttack(25).get();
    auto mage = builder.setName("Mage").setHealth(100).setAttack(40).get();

    auto heroUnit = std::make_shared<SingleUnit>(hero);
    auto mageUnit = std::make_shared<SingleUnit>(mage);

    auto party = std::make_shared<UnitGroup>();
    party->addUnit(heroUnit);
    party->addUnit(mageUnit);

    party->info();
    party->attack();

    SwordAttack sword;
    MagicAttack magic;

    std::cout << "\nHero attacks:\n";
    sword.performAttack();

    std::cout << "\nMage attacks:\n";
    magic.performAttack();
}
