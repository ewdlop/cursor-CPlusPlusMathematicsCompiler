#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <memory>
#include <stdexcept>

class Expr {
public:
    virtual double eval() const = 0;
    virtual ~Expr() = default;
};

class Number : public Expr {
    double value;
public:
    explicit Number(double v) : value(v) {}
    double eval() const override { return value; }
};

class BinaryOp : public Expr {
    char op;
    std::unique_ptr<Expr> left, right;
public:
    BinaryOp(char op, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(op), left(std::move(l)), right(std::move(r)) {
    }

    double eval() const override {
        switch (op) {
        case '+': return left->eval() + right->eval();
        case '-': return left->eval() - right->eval();
        case '*': return left->eval() * right->eval();
        case '/': return left->eval() / right->eval();
        default: throw std::runtime_error("Unknown operator");
        }
    }
};

class Parser {
    std::istringstream input;
    char current;

    void next() {
        input >> std::ws;
        current = input.peek();
    }

    char consume() {
        char ch = input.get();
        next();
        return ch;
    }

    std::unique_ptr<Expr> parsePrimary() {
        if (std::isdigit(current) || current == '.') {
            double val;
            input >> val;
            next();
            return std::make_unique<Number>(val);
        }
        if (current == '(') {
            consume();  // consume '('
            auto expr = parseExpression();
            if (current != ')') throw std::runtime_error("Expected ')'");
            consume();  // consume ')'
            return expr;
        }
        throw std::runtime_error("Unexpected character");
    }

    std::unique_ptr<Expr> parseTerm() {
        auto node = parsePrimary();
        while (current == '*' || current == '/') {
            char op = consume();
            auto right = parsePrimary();
            node = std::make_unique<BinaryOp>(op, std::move(node), std::move(right));
        }
        return node;
    }

    std::unique_ptr<Expr> parseExpression() {
        auto node = parseTerm();
        while (current == '+' || current == '-') {
            char op = consume();
            auto right = parseTerm();
            node = std::make_unique<BinaryOp>(op, std::move(node), std::move(right));
        }
        return node;
    }

public:
    explicit Parser(const std::string& text) : input(text) {
        next();
    }

    std::unique_ptr<Expr> parse() {
        return parseExpression();
    }
};

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
