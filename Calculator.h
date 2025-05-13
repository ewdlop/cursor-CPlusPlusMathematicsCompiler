#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <map>

// Define math constants if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// Forward declarations
class Expr;
class Number;
class BinaryOp;
class UnaryOp;
class Constant;
class Parser;

// Expression base class
class Expr {
public:
    virtual ~Expr() = default;
    virtual double eval() const = 0;
};

// Number class
class Number : public Expr {
    double value;
public:
    explicit Number(double val) : value(val) {}
    double eval() const override { return value; }
};

// Binary operation class
class BinaryOp : public Expr {
    char op;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
public:
    BinaryOp(char op, std::shared_ptr<Expr> l, std::shared_ptr<Expr> r)
        : op(op), left(l), right(r) {}
    
    double eval() const override {
        double l = left->eval();
        double r = right->eval();
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
};

// Unary operation class
class UnaryOp : public Expr {
    char op;
    std::shared_ptr<Expr> expr;
public:
    UnaryOp(char op, std::shared_ptr<Expr> e) : op(op), expr(e) {}
    
    double eval() const override {
        double val = expr->eval();
        switch (op) {
            case '-': return -val;
            default: throw std::runtime_error("Unknown unary operator");
        }
    }
};

// Constant class
class Constant : public Expr {
    std::string name;
public:
    explicit Constant(std::string n) : name(n) {}
    
    double eval() const override {
        if (name == "pi") return M_PI;
        if (name == "e") return M_E;
        throw std::runtime_error("Unknown constant: " + name);
    }
};

// Function call class
class FunctionCall : public Expr {
    std::string name;
    std::shared_ptr<Expr> arg;
public:
    FunctionCall(std::string n, std::shared_ptr<Expr> a) : name(n), arg(a) {}
    
    double eval() const override {
        double val = arg->eval();
        if (name == "sin") return std::sin(val);
        if (name == "cos") return std::cos(val);
        if (name == "tan") return std::tan(val);
        if (name == "sqrt") {
            if (val < 0) throw std::runtime_error("Square root of negative number");
            return std::sqrt(val);
        }
        throw std::runtime_error("Unknown function: " + name);
    }
};

// Parser class
class Parser {
    std::string input;
    size_t pos = 0;
    
    char peek() const { return pos < input.length() ? input[pos] : '\0'; }
    char advance() { return input[pos++]; }
    bool match(char c) {
        if (peek() == c) {
            advance();
            return true;
        }
        return false;
    }
    
    std::shared_ptr<Expr> parsePrimary();
    std::shared_ptr<Expr> parseUnary();
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parseTerm();
    std::shared_ptr<Expr> parseExpr();
    
public:
    explicit Parser(std::string expr) : input(std::move(expr)) {}
    std::shared_ptr<Expr> parse();
}; 