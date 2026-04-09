#include <iostream>
#include <vector>
#include <memory>

class Entity {
public:
    virtual ~Entity() { std::cout << "Entity destroyed\n"; }
    virtual std::unique_ptr<Entity> copy() const = 0;
};

class Client : public Entity {
public:
    std::unique_ptr<Entity> copy() const override {
        return std::make_unique<Client>(*this);
    }
};

class Server : public Entity {
public:
    std::unique_ptr<Entity> copy() const override {
        return std::make_unique<Server>(*this);
    }
};

class Prototype {
public:
    Prototype() {
        m_entities.push_back(std::make_unique<Client>());
        m_entities.push_back(std::make_unique<Server>());
    }

    // No need for manual destructor cleanup!
    
    std::unique_ptr<Entity> make_client() { return m_entities.at(0)->copy(); }
    std::unique_ptr<Entity> make_server() { return m_entities.at(1)->copy(); }

private:
    std::vector<std::unique_ptr<Entity>> m_entities;
};

int main() {
    Prototype proto;
    auto client = proto.make_client();
}
