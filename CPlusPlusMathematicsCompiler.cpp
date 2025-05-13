#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <cmath>
#include "Calculator.h"

int main() {
    std::string expr;
    std::cout << "Enter expression: ";
    std::getline(std::cin, expr);

    try {
        Parser parser(expr);
        auto ast = parser.parse();
        std::cout << "Result = " << ast->eval() << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}
