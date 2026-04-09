#include <iostream>
#include <source_location>

class Tracer {
public:
  Tracer(const std::source_location &loc = std::source_location::current())
      : location(loc) {
    std::cout << "[TRACE] ENTER: " << location.function_name() << " ("
              << location.file_name() << ":" << location.line() << ")"
              << std::endl;
  }

  ~Tracer() {
    std::cout << "[TRACE] EXIT:  " << location.function_name() << std::endl;
  }

private:
  std::source_location location;
};

#ifndef NDEBUG
#define trace() Tracer _tracer_instance(std::source_location::current())
#else
#define trace() ((void)0)
#endif

void function_b() {
  trace();
  std::cout << "  Inside function_b" << std::endl;
}

void function_a() {
  trace();
  std::cout << "  Inside function_a, calling function_b" << std::endl;
  function_b();
}

int main() {
  trace();
  std::cout << "Starting demonstration of Tracer..." << std::endl;

  function_a();

  std::cout << "Demonstration complete." << std::endl;
  return 0;
}
