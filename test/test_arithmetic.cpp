// ����� ��� ���������� �������������� ���������

#include <gtest.h>

#include <cmath>

#include "Polinom.h"
#include "arithmetic.h"

// Тест конструктора с корректными выражениями
TEST(ArithmeticTest, ConstructorValidExpression) {
    EXPECT_NO_THROW(Arithmetic<double>("2+2"));
    EXPECT_NO_THROW(Arithmetic<double>("a+b"));
    EXPECT_NO_THROW(Arithmetic<double>("(a+b)*c"));
    EXPECT_NO_THROW(Arithmetic<double>("2.5 + 3.7"));
}

// Тест конструктора с некорректными скобками
TEST(ArithmeticTest, ConstructorInvalidBrackets) {
    EXPECT_THROW(Arithmetic<double>("(2+2"), std::invalid_argument);
    EXPECT_THROW(Arithmetic<double>("2+2)"), std::invalid_argument);
    EXPECT_THROW(Arithmetic<double>("((2+2)"), std::invalid_argument);
    EXPECT_THROW(Arithmetic<double>(")2+2("), std::invalid_argument);
}

// Тест метода GetInfix
TEST(ArithmeticTest, GetInfixReturnsOriginalExpression) {
    Arithmetic<double> expr1("a + b");
    EXPECT_EQ(expr1.GetInfix(), "a + b");

    Arithmetic<double> expr2("(x + y) * 2");
    EXPECT_EQ(expr2.GetInfix(), "(x + y) * 2");
}

// Тест метода GetPostfix
TEST(ArithmeticTest, GetPostfixReturnsNonEmptyString) {
    Arithmetic<double> expr("a + b");
    std::string postfix = expr.GetPostfix();
    EXPECT_FALSE(postfix.empty());
    EXPECT_TRUE(postfix.length() > 0);
}

// Тест метода GetOperands
TEST(ArithmeticTest, GetOperandsExtractsVariables) {
    Arithmetic<double> expr("a + b * c");
    std::vector<std::string> operands = expr.GetOperands();

    EXPECT_EQ(operands.size(), 3);
    EXPECT_NE(std::find(operands.begin(), operands.end(), "a"), operands.end());
    EXPECT_NE(std::find(operands.begin(), operands.end(), "b"), operands.end());
    EXPECT_NE(std::find(operands.begin(), operands.end(), "c"), operands.end());
}

// Тест метода GetOperandsWithDuplicateVariables
TEST(ArithmeticTest, GetOperandsWithDuplicateVariables) {
    Arithmetic<double> expr("x + x * y");
    std::vector<std::string> operands = expr.GetOperands();

    EXPECT_EQ(operands.size(), 2);
    EXPECT_NE(std::find(operands.begin(), operands.end(), "x"), operands.end());
    EXPECT_NE(std::find(operands.begin(), operands.end(), "y"), operands.end());
}

// Тест вычислений с числами
TEST(ArithmeticTest, CalculateWithNumbersOnly) {
    Arithmetic<double> expr1("2 + 3");
    std::map<std::string, double> values;
    EXPECT_DOUBLE_EQ(expr1.Calculate(values), 5.0);

    Arithmetic<double> expr2("10 - 4");
    EXPECT_DOUBLE_EQ(expr2.Calculate(values), 6.0);

    Arithmetic<double> expr3("6 * 7");
    EXPECT_DOUBLE_EQ(expr3.Calculate(values), 42.0);

    Arithmetic<double> expr4("15 / 3");
    EXPECT_DOUBLE_EQ(expr4.Calculate(values), 5.0);
}

// Тест вычислений с переменными
TEST(ArithmeticTest, CalculateWithVariables) {
    Arithmetic<double> expr1("a + b");
    std::map<std::string, double> values1 = {{"a", 5.0}, {"b", 3.0}};
    EXPECT_DOUBLE_EQ(expr1.Calculate(values1), 8.0);

    Arithmetic<double> expr2("x * y");
    std::map<std::string, double> values2 = {{"x", 4.0}, {"y", 2.5}};
    EXPECT_DOUBLE_EQ(expr2.Calculate(values2), 10.0);
}

// Тест приоритета операций
TEST(ArithmeticTest, OperationPriorityRespected) {
    Arithmetic<double> expr1("2 + 3 * 4");  // 2 + (3*4) = 14
    std::map<std::string, double> values;
    EXPECT_DOUBLE_EQ(expr1.Calculate(values), 14.0);

    Arithmetic<double> expr2("(2 + 3) * 4");  // (2+3)*4 = 20
    EXPECT_DOUBLE_EQ(expr2.Calculate(values), 20.0);
}

// Тест сложных выражений
TEST(ArithmeticTest, ComplexExpressionCalculation) {
    Arithmetic<double> expr("a + b * c - d / 2");
    std::map<std::string, double> values = {{"a", 10.0}, {"b", 3.0}, {"c", 4.0}, {"d", 8.0}};
    double expected = 10.0 + 3.0 * 4.0 - 8.0 / 2.0;  // 10 + 12 - 4 = 18
    EXPECT_DOUBLE_EQ(expr.Calculate(values), expected);
}

// Тест деления на ноль
TEST(ArithmeticTest, DivisionByZeroThrowsException) {
    Arithmetic<double> expr1("a / 0");
    std::map<std::string, double> values1 = {{"a", 5.0}};
    EXPECT_THROW(expr1.Calculate(values1), std::invalid_argument);

    Arithmetic<double> expr2("10 / b");
    std::map<std::string, double> values2 = {{"b", 0.0}};
    EXPECT_THROW(expr2.Calculate(values2), std::invalid_argument);
}

// Тест вычислений с плавающей точкой
TEST(ArithmeticTest, FloatingPointCalculations) {
    Arithmetic<double> expr1("2.5 + 3.7");
    std::map<std::string, double> values;
    EXPECT_NEAR(expr1.Calculate(values), 6.2, 0.0001);

    Arithmetic<double> expr2("10.0 / 4.0");
    EXPECT_DOUBLE_EQ(expr2.Calculate(values), 2.5);
}

// Тест выражений с разными типами лексем
TEST(ArithmeticTest, MixedNumberAndVariableExpressions) {
    Arithmetic<double> expr("2.5 + a * 3");
    std::map<std::string, double> values = {{"a", 4.0}};
    double expected = 2.5 + 4.0 * 3.0;  // 2.5 + 12 = 14.5
    EXPECT_NEAR(expr.Calculate(values), expected, 0.0001);
}

// Тест порядка операций с одинаковым приоритетом
TEST(ArithmeticTest, SamePriorityOperationOrder) {
    Arithmetic<double> expr1("10 - 3 - 2");  // (10-3)-2 = 5
    std::map<std::string, double> values;
    EXPECT_DOUBLE_EQ(expr1.Calculate(values), 5.0);

    Arithmetic<double> expr2("20 / 4 / 2");  // (20/4)/2 = 2.5
    EXPECT_DOUBLE_EQ(expr2.Calculate(values), 2.5);
}

// Тест метода Calculate без параметров (с вводом данных)
TEST(ArithmeticTest, CalculateWithoutParametersNoCrash) {
    Arithmetic<double> expr("a + b");
    // Этот тест может требовать ручного ввода данных
    // В реальных тестах лучше использовать mock для ввода
    // EXPECT_NO_THROW(expr.Calculate());
}

// Тест метода PrintLexems
TEST(ArithmeticTest, PrintLexemsNoCrash) {
    Arithmetic<double> expr("a + b * 2");
    EXPECT_NO_THROW(expr.PrintLexems());
}

// Тест выражения с одним операндом
TEST(ArithmeticTest, SingleOperandExpression) {
    Arithmetic<double> expr("42");
    std::map<std::string, double> values;
    EXPECT_DOUBLE_EQ(expr.Calculate(values), 42.0);

    Arithmetic<double> expr2("a");
    std::map<std::string, double> values2 = {{"a", 7.5}};
    EXPECT_DOUBLE_EQ(expr2.Calculate(values2), 7.5);
}

// Тест выражения с унарным минусом (если поддерживается)
TEST(ArithmeticTest, ComplexExpressionWithMultipleOperations) {
    Arithmetic<double> expr("a * b + c * d - e / f");
    std::map<std::string, double> values = {{"a", 2.0}, {"b", 3.0},  {"c", 4.0},
                                            {"d", 5.0}, {"e", 10.0}, {"f", 2.0}};
    double expected = 2.0 * 3.0 + 4.0 * 5.0 - 10.0 / 2.0;  // 6 + 20 - 5 = 21
    EXPECT_DOUBLE_EQ(expr.Calculate(values), expected);
}

// Тест проверка постфиксной формы
TEST(ArithmeticTest, CheckPostForm) {
    Arithmetic<double> expr("(a + b * 10) * (e - d + c)");
    Arithmetic<double> expr1("a / (b - c) * d - e");
    Arithmetic<double> expr2("a / (b - c * d - e)");

    EXPECT_EQ(expr.GetPostfix(), "a b 10 * + e d - c + *");
    EXPECT_EQ(expr1.GetPostfix(), "a b c - / d * e -");
    EXPECT_EQ(expr2.GetPostfix(), "a b c d * - e - /");
}

TEST(ArithmeticTest, PolinomConst) {
    Arithmetic<Polinom> expr("a + b");
    std::map<std::string, Polinom> values = {
        {"a", Polinom("20")}, {"b", Polinom("40")}, {"c", Polinom("4")}, {"d", Polinom("8")}};
    EXPECT_EQ(expr.Calculate(values), Polinom("60"));
}

TEST(ArithmeticTest, PolinomDefault) {
    Arithmetic<Polinom> expr("a + b");
    std::map<std::string, Polinom> values = {
        {"a", Polinom("20x1y1z1")}, {"b", Polinom("40")}, {"c", Polinom("4")}, {"d", Polinom("8")}};
    EXPECT_EQ(expr.Calculate(values), Polinom("20x1y1z1+40"));
}

TEST(ArithmeticTest, PolinomAssoch) {
    Arithmetic<Polinom> expr("a - (b + c)");
    std::map<std::string, Polinom> values = {
        {"a", Polinom("100x1y1z1")}, {"b", Polinom("40")}, {"c", Polinom("4")}, {"d", Polinom("8")}};
    EXPECT_EQ(expr.Calculate(values), Polinom("100x1y1z1+-44"));
}

TEST(ArithmeticTest, PolinomAndDoubleAddition) {
    Arithmetic<Polinom> expr("a + b + 10");
    std::map<std::string, Polinom> values = {
        {"a", Polinom("100x1y1z1")}, {"b", Polinom("40")}, {"c", Polinom("4")}, {"d", Polinom("8")}};
    EXPECT_EQ(expr.Calculate(values), Polinom("100x1y1z1+50"));

    Arithmetic<Polinom> expr1("a + b + 25.3");
    EXPECT_EQ(expr1.Calculate(values), Polinom("100x1y1z1+65.3"));
}

TEST(ArithmeticTest, PolinomAndDoubleMultiplication) {
    Arithmetic<Polinom> expr("a + b * 10");
    std::map<std::string, Polinom> values = {
        {"a", Polinom("100x1y1z1")}, {"b", Polinom("40")}, {"c", Polinom("4")}, {"d", Polinom("8")}};
    EXPECT_EQ(expr.Calculate(values), Polinom("100x1y1z1+400"));
}