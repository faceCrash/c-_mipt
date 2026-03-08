#include <cassert>
#include <string>
#include <utility>

class Builder;

class Person
{
public:
    Person() = default;

    Person(std::string name, int age, int grade)
        : m_name(std::move(name))
        , m_age(age)
        , m_grade(grade)
    {}

    const std::string& name() const { return m_name; }
    int age()   const { return m_age; }
    int grade() const { return m_grade; }

private:
    std::string m_name;
    int m_age   = 0;
    int m_grade = 0;

    friend class Builder;
};

class Builder
{
public:
    Builder() = default;

    Builder& name(std::string name)
    {
        m_person.m_name = std::move(name);
        return *this;
    }

    Builder& age(int age)
    {
        m_person.m_age = age;
        return *this;
    }

    Builder& grade(int grade)
    {
        m_person.m_grade = grade;
        return *this;
    }

    Person get() const
    {
        return m_person;
    }

private:
    Person m_person;
};

int main()
{
    Builder builder;

    auto person = builder
        .name("Ivan")
        .age(25)
        .grade(10)
        .get();

    static_assert(std::is_same_v<decltype(person), Person>);

    assert(person.name()  == "Ivan");
    assert(person.age()   == 25);
    assert(person.grade() == 10);
}