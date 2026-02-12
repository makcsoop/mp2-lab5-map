#pragma once

#include <stdlib.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Polinom.h"
#include "stack.h"

using namespace std;

// 0 - операция
// 1 - переменная
// 2 - цифра
// 3 - полином ????
class Lexem {
    string name;
    int type;

   public:
    Lexem();
    Lexem(string _name, int _type);

    friend ostream &operator<<(ostream &ostr, const Lexem &s);

    string GetName();
    int GetType();
};

template <typename T>
class Arithmetic {
    vector<Lexem> lexems;
    vector<Lexem> postfix;
    string infix;
    map<string, int> priority = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"(", 0}, {")", 0}};
    // map<string, double> operands;
    map<string, T> operands;

   public:
    Arithmetic(string _infix);

    bool CorrectBrackets();
    void ToParsingLexem();
    void TranslationToPost();
    void InputOperands();

    T ConvertToT(const string &str);

    string GetInfix();
    string GetPostfix();
    vector<string> GetOperands();
    // double Calculate(map<string, double> values = {});
    T Calculate(map<string, T> values = {});
    void PrintLexems();
};

template <typename T>
Arithmetic<T>::Arithmetic(string _infix) : infix(_infix) {
    if (CorrectBrackets()) {
        ToParsingLexem();
        TranslationToPost();
    } else {
        throw invalid_argument("Error Correct Bracket");
    }
}
template <typename T>
bool Arithmetic<T>::CorrectBrackets() {
    Stake<string> check;
    int flag_operant = 1;
    string oper = "+-/*";
    int last_index;
    for (int i = 0; i < infix.length(); i++) {
        if (infix[i] == ' ') {  // пропускаем пробелы
            continue;
        }

        if (flag_operant && i < infix.length() - 1) {  // если только что была операция
            if (infix[i] == '.' ||
                infix[i] == ')' ||  // после операции не идет дробное и ) или после операции не идет .
                (i - 2 >= 0 && (infix[i - 2] == '(' || infix[i - 2] == '.'))) {
                return false;
            }
        }
        if (oper.find(infix[i]) != std::string::npos) {
            if (flag_operant) {  // две операции не идет подряд
                return false;
            } else {
                flag_operant = 1;
            }
        } else if (infix[i] == '(') {
            check.push("(");
            flag_operant = 0;
        } else if (infix[i] == ')') {
            if (check.is_empty() || infix[last_index] == '(') {  // то что есть закрывающая и нет ситуации ()
                return false;
            }
            check.pop();
            flag_operant = 0;
        } else {
            flag_operant = 0;
        }
        last_index = i;
    }
    if (!check.is_empty() || flag_operant) {
        return false;
    }
    return true;
}
template <typename T>
void Arithmetic<T>::ToParsingLexem() {
    int lenght = infix.size();
    string oper = "+-/*()";
    for (int i = 0; i < lenght; i++) {
        if (oper.find(infix[i]) != std::string::npos) {
            Lexem tmp(string(&infix[i], 1), 0);  // добавляем операцию
            lexems.push_back(tmp);
        } else if (isdigit(infix[i]) != 0) {
            string number = "";
            int flag = 1;
            while (i < lenght && (isdigit(infix[i]) != 0 ||
                                  infix[i] == '.')) {  // парсим число пока есть числа подряд или .
                if (flag && infix[i] == '.') {
                    flag = 0;
                } else if (!flag && infix[i] == '.') {  // если в дробном числе две точки
                    throw invalid_argument("two dot");
                }
                number += infix[i];
                i++;
            }
            i--;
            Lexem tmp(number, 2);
            lexems.push_back(Lexem(number, 2));

        } else if (isalnum(infix[i])) {
            Lexem tmp(string(&infix[i], 1), 1);  // добавляем переменную
            lexems.push_back(tmp);
        }
    }
}
template <typename T>
void Arithmetic<T>::TranslationToPost() {
    Stake<Lexem> stake;
    string result = "";
    string oper = "+-/*()";
    for (int i = 0; i < lexems.size(); i++) {
        if (oper.find(lexems[i].GetName()) != std::string::npos) {
            if (lexems[i].GetName() == "(") {
                Lexem tmp("(", 0);
                stake.push(tmp);
            } else if (lexems[i].GetName() == ")") {
                Lexem tmp = stake.pop();
                while (tmp.GetName() != "(") {  // достаем до закрывания скобки
                    postfix.push_back(tmp);
                    tmp = stake.pop();
                }

            } else {
                Lexem tmp = stake.top();
                while (!stake.is_empty() &&
                       (tmp.GetName() != "(" && (priority[tmp.GetName()] >= priority[lexems[i].GetName()]))) {
                    postfix.push_back(stake.pop());  // пока приоритет >= текущего против которого вытаскиваю
                    tmp = stake.top();
                }
                stake.push(lexems[i]);
            }
        } else {
            postfix.push_back(lexems[i]);  //  число добавляется
        }
    }
    while (!stake.is_empty()) {
        postfix.push_back(stake.pop());
    }
}

template <typename T>
void Arithmetic<T>::InputOperands() {
    for (Lexem lexem : lexems) {
        if (isalpha((lexem.GetName()[0]))) {
            if (operands.count(lexem.GetName()) == 0) {
                cout << "Input values " << lexem.GetName() << ": = ";
                cin >> operands[lexem.GetName()];
            }
        }
    }
}
template <typename T>
string Arithmetic<T>::GetInfix() {
    return infix;
}

template <typename T>
string Arithmetic<T>::GetPostfix() {
    string res = "";
    for (int i = 0; i < postfix.size(); i++) {
        res += postfix[i].GetName();
        if (i != postfix.size() - 1) {
            res += " ";
        }
    }
    return res;
}

template <typename T>
vector<string> Arithmetic<T>::GetOperands() {
    set<string> unique_operands;
    for (auto lexem : lexems) {
        if (isalpha(lexem.GetName()[0])) {
            unique_operands.insert(lexem.GetName());
        }
    }
    return vector<string>(unique_operands.begin(), unique_operands.end());
}

template <typename T>
T Arithmetic<T>::Calculate(map<string, T> values) {
    if (values.size() == 0) {
        InputOperands();
    } else {
        operands = values;
    }
    Stake<T> stake;
    for (auto lexem : postfix) {
        if (lexem.GetType() == 0) {
            T first = stake.pop(), second = stake.pop();
            switch (lexem.GetName()[0]) {
                case '+':
                    stake.push(first + second);
                    break;
                case '-':
                    stake.push(second - first);
                    break;
                case '*':
                    stake.push(first * second);
                    break;
                case '/':
                    if (first != 0) {
                        stake.push(second / first);
                    } else {
                        throw invalid_argument("Ошибка: деление на ноль");
                    }
                    break;
                default:
                    break;
            }
        } else if (lexem.GetType() == 1) {
            cout << operands[lexem.GetName()];
            stake.push(operands[lexem.GetName()]);
        } else {
            stake.push(ConvertToT(lexem.GetName()));
        }
    }
    return stake.pop();
}

template <typename T>
T Arithmetic<T>::ConvertToT(const string &str) {
    if constexpr (is_same_v<T, double>) {
        return stod(str);
    } else if constexpr (is_same_v<T, int>) {
        return stoi(str);
    } else if constexpr (is_same_v<T, Polinom>) {
        return Polinom(str);
    }
}

template <typename T>
void Arithmetic<T>::PrintLexems() {
    for (auto lexem : lexems) {
        cout << lexem.GetName();
    }
    cout << "\n";
}