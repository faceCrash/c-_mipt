#include <iostream>

class Wrapper;
using FuncPtr = Wrapper (*)();

class Wrapper {
public:
  Wrapper(FuncPtr p) : p_(p) {}

  operator FuncPtr() const { return p_; }

private:
  FuncPtr p_;
};

Wrapper test() {
  std::cout << "test() called\n";
  return test;
}

int main() {
  std::cout << "--- Task 11.01 ---" << std::endl;

  Wrapper function = test();
  (*function)();

  return 0;
}
