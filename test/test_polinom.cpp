#include <gtest.h>

#include <cmath>

#include "Polinom.h"

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc

TEST(PolinomTest, DefaultConstructor) {
    Polinom polinom;
    EXPECT_FALSE(polinom.IsEmpty());
    EXPECT_EQ(polinom.size(), 1);
}

TEST(PolinomTest, StringConstructor) {
    string pol = "200x2y2z2+10x1y2z3+20";
    Polinom polinom(pol);
    EXPECT_EQ(polinom.size(), 3);
    EXPECT_EQ(polinom[0].degrees, 222);
    EXPECT_EQ(polinom[1].degrees, 123);
    EXPECT_EQ(polinom[2].coeef, 20);
    EXPECT_EQ(polinom.Front().degrees, 222);
}

TEST(PolinomTest, Assignment) {
    string pol = "200x2y2z2+10x1y2z3+20";
    Polinom polinom(pol);
    Polinom test2 = polinom;
    EXPECT_EQ(test2.size(), 3);
    EXPECT_EQ(test2[1].degrees, 123);
    EXPECT_EQ(test2[2].degrees, 0);
    EXPECT_EQ(test2[2].coeef, 20);
    EXPECT_EQ(test2.Front().degrees, 222);

    EXPECT_EQ(polinom.size(), 3);
    EXPECT_EQ(polinom[1].degrees, 123);
    EXPECT_EQ(polinom[2].degrees, 0);
    EXPECT_EQ(polinom[2].coeef, 20);
    EXPECT_EQ(polinom.Front().degrees, 222);

    test2[1].degrees = 122;
    EXPECT_NE(test2[1].degrees, polinom[1].degrees);
    EXPECT_EQ(test2[0].degrees, polinom[0].degrees);
}

TEST(PolinomTest, OperatopPlus) {
    string pol = "600x3y3z3+200x2y2z2+10x1y2z3+20";
    string answer = "1200x3y3z3+400x2y2z2+20x1y2z3+40";
    Polinom polinom(pol);
    Polinom answer_pol(answer);
    Polinom pol2 = polinom + polinom;
    EXPECT_EQ(pol2.size(), polinom.size());
    EXPECT_EQ(pol2[0].degrees, polinom[0].degrees);
    EXPECT_EQ(pol2[1].degrees, polinom[1].degrees);
    EXPECT_EQ(pol2[0].coeef, polinom[0].coeef * 2);
    EXPECT_EQ(pol2[1].coeef, polinom[1].coeef * 2);
    EXPECT_EQ(pol2[2].coeef, polinom[2].coeef * 2);
    EXPECT_EQ(pol2.Print(), answer_pol.Print());

    string pol3 = "40";
    Polinom polinom2(pol3);
    Polinom pol4 = polinom2 + polinom2;
    EXPECT_EQ(pol4.size(), polinom2.size());
    EXPECT_EQ(pol4[0].coeef, 80);
}

TEST(PolinomTest, Print) {
    string pol = "200x2y2z2+10x1y2z3+20";
    Polinom polinom(pol);
    EXPECT_EQ(polinom.Print(), "200x2y2z2+10x1y2z3+20");
}

TEST(PolinomTest, MulOperation) {
    string pol = "600x3y3z3";
    Polinom polinom(pol);
    Polinom pol2 = polinom * polinom;
    EXPECT_EQ(pol2.size(), 1);

    string pol3 = "12x2y2z2+10x1y2z3+130";
    Polinom polinom2(pol3);
    Polinom pol4 = polinom2 * polinom2;
    EXPECT_EQ(pol4.size(), 6);
    EXPECT_EQ(pol4.Print(), "144x4y4z4+240x3y4z5+100x2y4z6+3120x2y2z2+2600x1y2z3+16900");

    Polinom constans("10");
    Polinom constans2("20");
    Polinom pol_const = constans * constans2;
    EXPECT_EQ(pol_const.size(), 1);
    EXPECT_EQ(pol_const.Front().coeef, 200);

    Polinom pol_const_na_pol = constans * polinom2;
    EXPECT_EQ(pol_const_na_pol.size(), polinom2.size());
    EXPECT_EQ(pol_const_na_pol.Print(), "120x2y2z2+100x1y2z3+1300");

    Polinom polinom3("50x2y2z2+10");
    Polinom pol5 = polinom3 * constans2;
    EXPECT_EQ(pol5.size(), 2);
    EXPECT_EQ(pol5.Front().coeef, 1000);
}

TEST(PolinomTest, OperatopPlusDeatnNood) {
    string pol = "600x3y3z3+200x2y2z2+10x1y2z3+20";
    string pol2 = "600x3y3z2+-200x2y2z2+10x1y2z3+20";
    string answer = "600x3y3z3+600x3y3z2+20x1y2z3+40";
    Polinom polinom(pol);
    Polinom polino2(pol2);
    Polinom ans(answer);
    Polinom pol3 = polinom + polino2;
    EXPECT_EQ(pol3.size(), ans.size());
    EXPECT_EQ(pol3.Print(), ans.Print());
    EXPECT_EQ(pol3[0].degrees, ans[0].degrees);
    EXPECT_EQ(pol3[1].degrees, ans[1].degrees);
    EXPECT_DOUBLE_EQ(pol3[0].coeef, polinom[0].coeef);
    EXPECT_DOUBLE_EQ(pol3[1].coeef, polinom[0].coeef);
}

TEST(PolinomTest, OperatirMinus) {
    string pol = "600x3y3z3+200x2y2z2+10x1y2z3+20";
    Polinom polinom(pol);
    string pol2 = "600x3y3z3+-200x2y2z2+10x1y2z3+20";
    Polinom polinom2(pol2);
    Polinom pol3 = polinom - polinom2;
    EXPECT_EQ(pol3.size(), 1);
    EXPECT_EQ(pol3[0].coeef, 400);
    EXPECT_EQ(pol3.Print(), "400x2y2z2");

    string pol5 = "600x3y3z2+200x2y2z2+10x1y2z3";
    Polinom polinom3(pol5);
    Polinom pol4 = polinom - polinom3;
    EXPECT_EQ(pol4.size(), 3);
    EXPECT_EQ(pol4.Print(), "600x3y3z3+-600x3y3z2+20");
}

TEST(PolinomTest, MonomConstructor) {
    Monom m{5.5, 123};
    Polinom polinom(m);
    EXPECT_EQ(polinom.size(), 1);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 5.5);
    EXPECT_EQ(polinom[0].degrees, 123);
}

TEST(PolinomTest, EmptyStringConstructor) {
    Polinom polinom("");
    EXPECT_EQ(polinom.size(), 1);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 0);
    EXPECT_EQ(polinom[0].degrees, 0);
}

TEST(PolinomTest, StringConstructorWithNegativeCoefficients) {
    string pol = "-200x2y2z2+-10x1y2z3+-20";
    Polinom polinom(pol);
    EXPECT_EQ(polinom.size(), 3);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, -200);
    EXPECT_DOUBLE_EQ(polinom[1].coeef, -10);
    EXPECT_DOUBLE_EQ(polinom[2].coeef, -20);
}

TEST(PolinomTest, StringConstructorWithSpaces) {
    string pol = "200x2y2z2 + 10x1y2z3 + 20";
    Polinom polinom(pol);
    EXPECT_EQ(polinom.size(), 3);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 200);
    EXPECT_DOUBLE_EQ(polinom[1].coeef, 10);
    EXPECT_DOUBLE_EQ(polinom[2].coeef, 20);
}

TEST(PolinomTest, CopyConstructor) {
    string pol = "300x3y3z3+150x2y2z2+75";
    Polinom polinom1(pol);
    Polinom polinom2(polinom1);

    EXPECT_EQ(polinom2.size(), polinom1.size());
    EXPECT_DOUBLE_EQ(polinom2[0].coeef, polinom1[0].coeef);
    EXPECT_EQ(polinom2[0].degrees, polinom1[0].degrees);
    EXPECT_DOUBLE_EQ(polinom2[1].coeef, polinom1[1].coeef);
    EXPECT_EQ(polinom2[1].degrees, polinom1[1].degrees);

    polinom2[0].coeef = 999;
    EXPECT_NE(polinom2[0].coeef, polinom1[0].coeef);
}

TEST(PolinomTest, SelfAssignment) {
    string pol = "100x2y2z2+50x1y1z1+25";
    Polinom polinom(pol);
    polinom = polinom;

    EXPECT_EQ(polinom.size(), 3);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 100);
    EXPECT_DOUBLE_EQ(polinom[1].coeef, 50);
    EXPECT_DOUBLE_EQ(polinom[2].coeef, 25);
}

TEST(PolinomTest, OperatorPlusWithEmpty) {
    Polinom empty;
    string pol = "100x2y2z2+50";
    Polinom nonEmpty(pol);

    Polinom result1 = empty + nonEmpty;
    EXPECT_EQ(result1.size(), 2);
    EXPECT_DOUBLE_EQ(result1[0].coeef, 100);
    EXPECT_DOUBLE_EQ(result1[1].coeef, 50);

    Polinom result2 = nonEmpty + empty;
    EXPECT_EQ(result2.size(), 2);
    EXPECT_DOUBLE_EQ(result2[0].coeef, 100);
    EXPECT_DOUBLE_EQ(result2[1].coeef, 50);
}

TEST(PolinomTest, OperatorPlusEqualsMonom) {
    Polinom polinom;
    Monom m1{10.5, 111};
    Monom m2{-5.5, 222};

    polinom += m1;
    EXPECT_EQ(polinom.size(), 2);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 10.5);
    EXPECT_EQ(polinom[0].degrees, 111);
    polinom += m2;
    EXPECT_EQ(polinom.size(), 3);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, -5.5);
    EXPECT_EQ(polinom[0].degrees, 222);
}

TEST(PolinomTest, OperatorMultiplyByZero) {
    string pol = "100x2y2z2+50x1y1z1+25";
    Polinom polinom(pol);
    Polinom zero;

    Polinom result = polinom * zero;
    EXPECT_EQ(result.size(), 1);
    EXPECT_DOUBLE_EQ(result[0].coeef, 0);
    EXPECT_EQ(result[0].degrees, 0);
}

TEST(PolinomTest, OperatorMultiplyCommutative) {
    string pol1 = "2x1+1";
    string pol2 = "3x1+2";
    Polinom p1(pol1);
    Polinom p2(pol2);

    Polinom result1 = p1 * p2;
    Polinom result2 = p2 * p1;

    EXPECT_EQ(result1.Print(), result2.Print());
}

TEST(PolinomTest, OperatorMinusWithSelf) {
    string pol = "100x2y2z2+50x1y1z1+25";
    Polinom polinom(pol);
    Polinom result = polinom - polinom;
    EXPECT_EQ(result.size(), 1);
    EXPECT_DOUBLE_EQ(result[0].coeef, 0);
    EXPECT_EQ(result[0].degrees, 0);
}

TEST(PolinomTest, OperatorMinusWithDifferentDegrees) {
    string pol1 = "100x3y3z3+50x2y2z2";
    string pol2 = "30x2y2z2+20x1y1z1";
    Polinom p1(pol1);
    Polinom p2(pol2);

    Polinom result = p1 - p2;
    EXPECT_EQ(result.size(), 3);
    EXPECT_DOUBLE_EQ(result[0].coeef, 100);
    EXPECT_EQ(result[0].degrees, 333);
    EXPECT_DOUBLE_EQ(result[1].coeef, 20);
    EXPECT_EQ(result[1].degrees, 222);
    EXPECT_DOUBLE_EQ(result[2].coeef, -20);
    EXPECT_EQ(result[2].degrees, 111);
}

TEST(PolinomTest, PrintWithZeroCoefficient) {
    Monom m1{5.0, 111};
    Monom m2{0.0, 222};
    Monom m3{3.0, 0};

    Polinom polinom;
    polinom += m1;
    polinom += m2;
    polinom += m3;

    string result = polinom.Print();
    EXPECT_EQ(result, "5x1y1z1+3");
}

TEST(PolinomTest, PrintWithSingleMonom) {
    Polinom polinom(Monom{7.5, 123});
    EXPECT_EQ(polinom.Print(), "7.5x1y2z3");
}

TEST(PolinomTest, Destructor) {
    Polinom* polinom = new Polinom("100x2y2z2+50x1y1z1+25");
    delete polinom;
    SUCCEED();
}

TEST(PolinomTest, MonomWithZeroDegree) {
    string pol = "5x0y0z0+3x1y0z0+2x0y1z0+1x0y0z1";
    Polinom polinom(pol);

    EXPECT_EQ(polinom.size(), 4);
    EXPECT_DOUBLE_EQ(polinom[0].coeef, 5);
    EXPECT_EQ(polinom[0].degrees, 0);
    EXPECT_DOUBLE_EQ(polinom[1].coeef, 3);
    EXPECT_EQ(polinom[1].degrees, 100);
    EXPECT_DOUBLE_EQ(polinom[2].coeef, 2);
    EXPECT_EQ(polinom[2].degrees, 10);
    EXPECT_DOUBLE_EQ(polinom[3].coeef, 1);
    EXPECT_EQ(polinom[3].degrees, 1);
}

TEST(PolinomTest, PolinomDouble) {
    string pol = "50.2x1y1z1+20";

    Polinom polinom(pol);
    EXPECT_EQ(polinom[0].coeef, 50.2);
    EXPECT_EQ(polinom.size(), 2);
    cout << polinom.Print();
}