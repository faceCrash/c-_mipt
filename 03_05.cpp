#include <cassert>

class Entity_v1 {
public:
    virtual ~Entity_v1() {}
    virtual void test() const = 0;
};

class Entity_v2 {
public:
    virtual ~Entity_v2() {}
    virtual void test() const = 0;
};

class Adapter_v1 : public virtual Entity_v1 {
public:
    virtual void test_v1() const = 0;
    void test() const override { test_v1(); }
    ~Adapter_v1() override {}
};

class Adapter_v2 : public virtual Entity_v2 {
public:
    virtual void test_v2() const = 0;
    void test() const override { test_v2(); }
    ~Adapter_v2() override {}
};

class Client final : public Adapter_v1, public Adapter_v2 {
public:
    void test_v1() const override { assert(1 + 1 == 2); }
    void test_v2() const override { assert(2 * 2 == 4); }
    ~Client() override {}
};

int main() {
    Client c;
    Entity_v1* v1 = &c;
    Entity_v2* v2 = &c;
    v1->test();
    v2->test();
    Adapter_v1* a1 = &c;
    Adapter_v2* a2 = &c;
    a1->test_v1();
    a2->test_v2();
    assert(dynamic_cast<Entity_v1*>(v1));
    assert(dynamic_cast<Entity_v2*>(v2));
    return 0;
}
