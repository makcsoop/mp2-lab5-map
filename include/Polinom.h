#pragma once
#include <map>
#include <sstream>
#include <vector>

#include "TSimpleList.h"

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc  src/*.cpp
vector<string> split(string str, char delimiter);
map<string, double> parseMonom(string Monom);
string format_double(double value);
void log(string message, int type = 0);

struct Monom {
    double coeef;
    int degrees;
} typedef Monom;

namespace Colors {
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string BOLD = "\033[1m";
}  // namespace Colors

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
