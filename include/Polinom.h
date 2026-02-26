#pragma once
#include <map>
#include <sstream>
#include <vector>

#include "TSimpleList.h"
#include "log.h"

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc  src/*.cpp
vector<string> split(string str, char delimiter);
map<string, double> parseMonom(string Monom);
string format_double(double value);

struct Monom {
    double coeef;
    int degrees;
} typedef Monom;

class Polinom : public TSimpleList<Monom> {
   private:
   public:
    Polinom();
    Polinom(Monom monom);
    Polinom(string pol_str);
    Polinom(double number);

    Polinom operator+(const Polinom& pol);
    Polinom operator+=(const Monom& monom);
    Polinom operator*(const Polinom& pol);
    Polinom operator-(const Polinom& pol);  // самониучтожение добавить
    Polinom operator/(const Polinom& pol);

    Polinom operator+(double pol);
    Polinom operator-(double pol);
    Polinom operator*(double pol);

    bool operator!=(int number);
    bool operator==(const Polinom& pol) const;

    ~Polinom();

    string Print();
    string Print() const;

    friend ostream& operator<<(ostream& osr, const Polinom& pol);
    friend istream& operator>>(istream& ist, const Polinom& pol);
};
