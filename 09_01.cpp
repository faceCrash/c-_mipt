#include <iostream>
#include <source_location>

/**
 * @brief Class Tracer represents a function call tracer using RAII pattern.
 */
class Tracer {
public:
    // Custom constructor that prints an entry message.
    // std::source_location::current() is passed by the caller to identify the call site.
    Tracer(const std::source_location& loc = std::source_location::current()) 
        : location(loc) {
        std::cout << "[TRACE] ENTER: " << location.function_name() 
                  << " (" << location.file_name() << ":" << location.line() << ")" << std::endl;
    }

    // Custom destructor that prints an exit message.
    ~Tracer() {
        std::cout << "[TRACE] EXIT:  " << location.function_name() << std::endl;
    }

private:
    std::source_location location;
};

/**
 * @brief Functional macro trace that allows disabling tracing if NDEBUG is defined.
 * 
 * Mimics the behavior of assert(). If NDEBUG is not defined, it creates a local Tracer 
 * instance that captures current source location information.
 */
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
