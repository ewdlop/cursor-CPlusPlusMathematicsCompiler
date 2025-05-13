#include "Calculator.h"
#include <sstream>
#include <cctype>

std::shared_ptr<Expr> Parser::parsePrimary() {
    if (isdigit(peek()) || peek() == '.') {
        std::string num;
        while (isdigit(peek()) || peek() == '.') {
            num += advance();
        }
        return std::make_shared<Number>(std::stod(num));
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

        throw std::runtime_error("Unknown identifier: " + id);
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

std::shared_ptr<Expr> Parser::parse() {
    return parseExpr();
} 