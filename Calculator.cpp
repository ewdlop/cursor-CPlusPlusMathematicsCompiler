#include "Calculator.h"
#include <sstream>
#include <cctype>
#include <regex>

// Assignment operation class
class Assignment : public Expr {
    std::string name;
    std::shared_ptr<Expr> value;
    SymbolTable& symbols;
public:
    Assignment(std::string n, std::shared_ptr<Expr> v, SymbolTable& s)
        : name(std::move(n)), value(std::move(v)), symbols(s) {}
    
    double eval() const override {
        double result = value->eval(symbols);
        symbols.set(name, result);
        return result;
    }
    
    double eval(const SymbolTable& s) const override {
        return eval();
    }
};

std::shared_ptr<Expr> Parser::parsePrimary() {
    if (isdigit(peek()) || peek() == '.') {
        std::string num;
        bool hasDecimal = false;
        bool hasExponent = false;
        
        // Parse the number
        while (isdigit(peek()) || peek() == '.' || peek() == 'e' || peek() == 'E' || peek() == '+' || peek() == '-') {
            char c = peek();
            
            // Handle decimal point
            if (c == '.') {
                if (hasDecimal) {
                    throw std::runtime_error("Multiple decimal points in number");
                }
                hasDecimal = true;
            }
            
            // Handle scientific notation
            if (c == 'e' || c == 'E') {
                if (hasExponent) {
                    throw std::runtime_error("Multiple exponents in number");
                }
                hasExponent = true;
                num += advance();
                
                // Handle optional sign after exponent
                if (peek() == '+' || peek() == '-') {
                    num += advance();
                }
                
                // Require at least one digit after exponent
                if (!isdigit(peek())) {
                    throw std::runtime_error("Expected digit after exponent");
                }
                continue;
            }
            
            num += advance();
        }
        
        try {
            return std::make_shared<Number>(std::stod(num));
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid number format: " + num);
        }
    }

    if (isalpha(peek())) {
        std::string id;
        while (isalnum(peek()) || peek() == '_') {
            id += advance();
        }

        if (id == "pi" || id == "e") {
            return std::make_shared<Constant>(id);
        }

        if (id == "sin" || id == "cos" || id == "tan" || id == "sqrt") {
            if (peek() != '(') {
                throw std::runtime_error("Expected '(' after function");
            }
            advance(); // consume '('
            auto arg = parseExpr();
            if (peek() != ')') {
                throw std::runtime_error("Expected ')' after function argument");
            }
            advance(); // consume ')'
            return std::make_shared<FunctionCall>(id, arg);
        }

        // Handle variable
        return std::make_shared<Variable>(id);
    }

    if (peek() == '(') {
        advance(); // consume '('
        auto expr = parseExpr();
        if (peek() != ')') {
            throw std::runtime_error("Expected ')'");
        }
        advance(); // consume ')'
        return expr;
    }

    if (peek() == '[') {
        advance(); // consume '['
        auto expr = parseExpr();
        if (peek() != ']') {
            throw std::runtime_error("Expected ']'");
        }
        advance(); // consume ']'
        return expr;
    }

    if (peek() == '{') {
        advance(); // consume '{'
        auto expr = parseExpr();
        if (peek() != '}') {
            throw std::runtime_error("Expected '}'");
        }
        advance(); // consume '}'
        return expr;
    }

    throw std::runtime_error("Unexpected character");
}

std::shared_ptr<Expr> Parser::parseUnary() {
    if (peek() == '-') {
        advance(); // consume '-'
        return std::make_shared<UnaryOp>('-', parsePrimary());
    }
    return parsePrimary();
}

std::shared_ptr<Expr> Parser::parseFactor() {
    auto expr = parseUnary();
    while (peek() == '^') {
        advance(); // consume '^'
        expr = std::make_shared<BinaryOp>('^', expr, parseUnary());
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();
    while (peek() == '*' || peek() == '/') {
        char op = advance(); // consume operator
        expr = std::make_shared<BinaryOp>(op, expr, parseFactor());
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseExpr() {
    auto expr = parseTerm();
    while (peek() == '+' || peek() == '-') {
        char op = advance(); // consume operator
        expr = std::make_shared<BinaryOp>(op, expr, parseTerm());
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseAssignment() {
    auto expr = parseExpr();
    
    if (peek() == '=') {
        // Check if the left side is a variable
        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            advance(); // consume '='
            auto value = parseExpr();
            return std::make_shared<Assignment>(var->getName(), value, symbols);
        }
        throw std::runtime_error("Left side of assignment must be a variable");
    }
    
    return expr;
}

std::shared_ptr<Expr> Parser::parse() {
    return parseAssignment();
}

// Update other classes to support symbol table
double BinaryOp::eval(const SymbolTable& symbols) const {
    double l = left->eval(symbols);
    double r = right->eval(symbols);
    switch (op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/': 
            if (r == 0) throw std::runtime_error("Division by zero");
            return l / r;
        case '^': return std::pow(l, r);
        default: throw std::runtime_error("Unknown operator");
    }
}

double UnaryOp::eval(const SymbolTable& symbols) const {
    double val = expr->eval(symbols);
    switch (op) {
        case '-': return -val;
        default: throw std::runtime_error("Unknown unary operator");
    }
}

double FunctionCall::eval(const SymbolTable& symbols) const {
    double val = arg->eval(symbols);
    if (name == "sin") return std::sin(val);
    if (name == "cos") return std::cos(val);
    if (name == "tan") return std::tan(val);
    if (name == "sqrt") {
        if (val < 0) throw std::runtime_error("Square root of negative number");
        return std::sqrt(val);
    }
    throw std::runtime_error("Unknown function: " + name);
} 