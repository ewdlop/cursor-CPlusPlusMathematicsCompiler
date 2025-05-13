#include <gtest/gtest.h>
#include "Calculator.h"

// Test fixture for calculator tests
class CalculatorTest : public ::testing::Test {
protected:
    double evaluate(const std::string& expr) {
        Parser parser(expr);
        auto ast = parser.parse();
        return ast->eval();
    }
};

// Basic arithmetic tests
TEST_F(CalculatorTest, BasicAddition) {
    EXPECT_DOUBLE_EQ(evaluate("2 + 2"), 4.0);
    EXPECT_DOUBLE_EQ(evaluate("5 + 3"), 8.0);
    EXPECT_DOUBLE_EQ(evaluate("-2 + 2"), 0.0);
}

TEST_F(CalculatorTest, BasicSubtraction) {
    EXPECT_DOUBLE_EQ(evaluate("5 - 3"), 2.0);
    EXPECT_DOUBLE_EQ(evaluate("3 - 5"), -2.0);
    EXPECT_DOUBLE_EQ(evaluate("0 - 0"), 0.0);
}

TEST_F(CalculatorTest, BasicMultiplication) {
    EXPECT_DOUBLE_EQ(evaluate("4 * 3"), 12.0);
    EXPECT_DOUBLE_EQ(evaluate("-2 * 3"), -6.0);
    EXPECT_DOUBLE_EQ(evaluate("0 * 5"), 0.0);
}

TEST_F(CalculatorTest, BasicDivision) {
    EXPECT_DOUBLE_EQ(evaluate("10 / 2"), 5.0);
    EXPECT_DOUBLE_EQ(evaluate("5 / 2"), 2.5);
    EXPECT_DOUBLE_EQ(evaluate("-6 / 2"), -3.0);
}

// Order of operations tests
TEST_F(CalculatorTest, OrderOfOperations) {
    EXPECT_DOUBLE_EQ(evaluate("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(evaluate("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate("2 * 3 + 4 * 5"), 26.0);
    EXPECT_DOUBLE_EQ(evaluate("2 + 3 * 4 + 5"), 19.0);
}

// Bracket tests
TEST_F(CalculatorTest, DifferentBrackets) {
    EXPECT_DOUBLE_EQ(evaluate("[2 + 3] * 4"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate("{2 + 3} * 4"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(evaluate("[(2 + 3) * 4]"), 20.0);
}

// Exponentiation tests
TEST_F(CalculatorTest, Exponentiation) {
    EXPECT_DOUBLE_EQ(evaluate("2^3"), 8.0);
    EXPECT_DOUBLE_EQ(evaluate("2^0.5"), std::sqrt(2));
    EXPECT_DOUBLE_EQ(evaluate("2^3^2"), 512.0);
    EXPECT_DOUBLE_EQ(evaluate("(2^3)^2"), 64.0);
}

// Trigonometric function tests
TEST_F(CalculatorTest, TrigonometricFunctions) {
    EXPECT_DOUBLE_EQ(evaluate("sin(0)"), 0.0);
    EXPECT_DOUBLE_EQ(evaluate("sin(pi/2)"), 1.0);
    EXPECT_DOUBLE_EQ(evaluate("cos(0)"), 1.0);
    EXPECT_DOUBLE_EQ(evaluate("cos(pi)"), -1.0);
    EXPECT_DOUBLE_EQ(evaluate("tan(0)"), 0.0);
}

// Square root tests
TEST_F(CalculatorTest, SquareRoot) {
    EXPECT_DOUBLE_EQ(evaluate("sqrt(16)"), 4.0);
    EXPECT_DOUBLE_EQ(evaluate("sqrt(2)"), std::sqrt(2));
    EXPECT_DOUBLE_EQ(evaluate("sqrt(0)"), 0.0);
}

// Constant tests
TEST_F(CalculatorTest, Constants) {
    EXPECT_DOUBLE_EQ(evaluate("pi"), M_PI);
    EXPECT_DOUBLE_EQ(evaluate("e"), M_E);
    EXPECT_DOUBLE_EQ(evaluate("2 * pi"), 2 * M_PI);
    EXPECT_DOUBLE_EQ(evaluate("e^2"), std::exp(2));
}

// Unary operation tests
TEST_F(CalculatorTest, UnaryOperations) {
    EXPECT_DOUBLE_EQ(evaluate("-5"), -5.0);
    EXPECT_DOUBLE_EQ(evaluate("-sin(pi/4)"), -std::sin(M_PI/4));
    EXPECT_DOUBLE_EQ(evaluate("--5"), 5.0);
}

// Complex expression tests
TEST_F(CalculatorTest, ComplexExpressions) {
    EXPECT_DOUBLE_EQ(evaluate("sin(pi/4) * sqrt(2)"), 1.0);
    EXPECT_DOUBLE_EQ(evaluate("2^3 + sin(pi/2)"), 9.0);
    EXPECT_DOUBLE_EQ(evaluate("-5 + e^2"), -5 + std::exp(2));
    EXPECT_DOUBLE_EQ(evaluate("sqrt(sin(pi/2)^2 + cos(pi/2)^2)"), 1.0);
}

// Error handling tests
TEST_F(CalculatorTest, DivisionByZero) {
    EXPECT_THROW(evaluate("1/0"), std::runtime_error);
}

TEST_F(CalculatorTest, SquareRootOfNegative) {
    EXPECT_THROW(evaluate("sqrt(-1)"), std::runtime_error);
}

TEST_F(CalculatorTest, InvalidExpression) {
    EXPECT_THROW(evaluate("2 + "), std::runtime_error);
    EXPECT_THROW(evaluate("(2 + 3"), std::runtime_error);
    EXPECT_THROW(evaluate("[2 + 3)"), std::runtime_error);
    EXPECT_THROW(evaluate("{2 + 3]"), std::runtime_error);
}

TEST_F(CalculatorTest, UnknownIdentifier) {
    EXPECT_THROW(evaluate("unknown(2)"), std::runtime_error);
}

TEST_F(CalculatorTest, InvalidFunctionCall) {
    EXPECT_THROW(evaluate("sin 2"), std::runtime_error);
}

TEST_F(CalculatorTest, InvalidOperator) {
    EXPECT_THROW(evaluate("2^^3"), std::runtime_error);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
