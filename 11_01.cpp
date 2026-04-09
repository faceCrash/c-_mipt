#include <iostream>

/**
 * @brief Item 11.01: Implementing a function that returns a pointer to itself.
 * This is achieved using a wrapper class to break the circular dependency in the function's type signature.
 */

class Wrapper;
using FuncPtr = Wrapper (*)();

/**
 * @brief Wrapper class that encapsulates a function pointer.
 * It provides a conversion operator to the function pointer type to allow
 * the wrapper to be used where a function pointer is expected.
 */
class Wrapper {
public:
    // Constructor allows implicit conversion from function pointer
    Wrapper(FuncPtr p) : p_(p) {}

    // Overloaded type conversion operator to function pointer
    // This allows (*function)() to work as requested
    operator FuncPtr() const { return p_; }

private:
    FuncPtr p_;
};

/**
 * @brief Example function that returns itself.
 */
Wrapper test() {
    std::cout << "test() called\n";
    return test;
}

int main() {
    std::cout << "--- Task 11.01 ---" << std::endl;
    
    // The requirement: Wrapper function = test(); (*function)();
    Wrapper function = test();
    (*function)();

    return 0;
}
