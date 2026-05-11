#include <iostream>

// This is a single line comment
/* This is a 
   multi-line 
   comment */

int main() {
    // Another comment
    
    std::cout << "Hello" << std::endl; // Comment after code

    /* Comment before code */ int x = 10;

    std::cout << R"(
        This is a raw string
        It has blank lines:
        
        And it has things that look like comments:
        // not a comment
        /* not a comment */
    )" << std::endl;

    return 0;
}

// Final blank lines and comments
    
// end
