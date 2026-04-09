#include <cstddef>
#include <new>
#include <iostream>

template <typename D>
class Entity {
public:
    // Standard operator new
    static void* operator new(std::size_t size) {
        std::cout << "Entity::operator new (" << size << " bytes)\n";
        return ::operator new(size);
    }

    // Standard operator delete
    static void operator delete(void* x) noexcept {
        std::cout << "Entity::operator delete\n";
        ::operator delete(x);
    }

    // Standard operator new[] (Array)
    static void* operator new[](std::size_t size) {
        std::cout << "Entity::operator new[] (" << size << " bytes)\n";
        return ::operator new[](size);
    }

    // Standard operator delete[] (Array)
    static void operator delete[](void* x) noexcept {
        std::cout << "Entity::operator delete[]\n";
        ::operator delete[](x);
    }

    // Nothrow operator new
    static void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
        std::cout << "Entity::operator new (nothrow, " << size << " bytes)\n";
        return ::operator new(size, std::nothrow);
    }

    // Nothrow operator delete (balancing version for construction failure)
    static void operator delete(void* x, const std::nothrow_t&) noexcept {
        std::cout << "Entity::operator delete (nothrow)\n";
        ::operator delete(x);
    }

    // Nothrow operator new[] (Array)
    static void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
        std::cout << "Entity::operator new[] (nothrow, " << size << " bytes)\n";
        return ::operator new[](size, std::nothrow);
    }

    // Nothrow operator delete[] (Array, balancing)
    static void operator delete[](void* x, const std::nothrow_t&) noexcept {
        std::cout << "Entity::operator delete[] (nothrow)\n";
        ::operator delete[](x);
    }

protected:
    Entity() = default;
};

class Client : private Entity<Client> {
public:
    Client() { std::cout << "Client::Client\n"; }
    ~Client() { std::cout << "Client::~Client\n"; }

    // Using declarations to expose operators
    using Entity<Client>::operator new;
    using Entity<Client>::operator delete;
    using Entity<Client>::operator new[];
    using Entity<Client>::operator delete[];
};

int main() {
    std::cout << "--- Testing single new ---\n";
    delete new Client;

    std::cout << "\n--- Testing array new ---\n";
    delete[] new Client[2];

    std::cout << "\n--- Testing nothrow single new ---\n";
    Client* p1 = new (std::nothrow) Client;
    delete p1;

    std::cout << "\n--- Testing nothrow array new ---\n";
    Client* p2 = new (std::nothrow) Client[2];
    delete[] p2;

    return 0;
}
