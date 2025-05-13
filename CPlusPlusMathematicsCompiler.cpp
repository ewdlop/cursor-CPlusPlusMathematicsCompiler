#include "Calculator.h"
#include <iostream>
#include <string>

int main() {
    SymbolTable symbols;
    std::string input;
    
    std::cout << "C++ Math Expression Compiler\n";
    std::cout << "Enter expressions (empty line to exit):\n";
    std::cout << "Examples:\n";
    std::cout << "  2 + 3 * 4\n";
    std::cout << "  x = 5\n";
    std::cout << "  x * 2\n";
    std::cout << "  sin(pi/2)\n";
    std::cout << "  1.23e-4\n";
    std::cout << "  sqrt(16)\n\n";
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input.empty()) {
            break;
        }
        
        try {
            Parser parser(input, symbols);
            auto expr = parser.parse();
            double result = expr->eval(symbols);
            std::cout << "= " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    
    return 0;
}
