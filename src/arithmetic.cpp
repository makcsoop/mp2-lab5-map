// реализация функций и классов для вычисления арифметических выражений
#include "../include/arithmetic.h"

Lexem::Lexem() : name("-1"), type(-1) {}

Lexem::Lexem(string _name, int _type) : name(_name), type(_type) {}

ostream &operator<<(ostream &ostr, const Lexem &s) {
    ostr << s.name;
    return ostr;
}

string Lexem::GetName() { return name; }
int Lexem::GetType() { return type; }