// реализация пользовательского приложения

#include <iostream>

#include "../include/arithmetic.h"
#include "../include/stack.h"

using namespace std;

void p(const std::string& a) { std::cout << a << endl; }

int main() {
    Stake<int> stake;
    for(int i = 0; i < 10; i++)
      stake.push(i);
    for(int i = 0; i < 10; i++){
      cout << stake.is_empty() << endl;
      cout << stake.pop() << "\n";
      cout << "len " << stake.length() << endl;
    }
    cout << stake.is_empty();

    Arithmetic test("(121+112.2)*2");
    cout << test.GetPostfix() << endl;
    test.PrintLexems();

    Arithmetic test2("1 + 1");
    cout << test2.GetPostfix() << endl;

    Arithmetic test3("(1 + 2 * 10) * (10 - 2 + a)");
    cout << test3.GetPostfix() << endl;
    cout << test3.GetInfix() << endl;
    cout << test3.Calculate() << endl;
    // test2.PrintLexems();

    Arithmetic test4("(a + b) * c - d / (e + f)");
    cout << test4.GetPostfix() << endl;
    cout << test4.Calculate() << endl;

    string exprStr;
    cout << "Enter expression : ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, exprStr);
    Arithmetic expr(exprStr);
    cout << expr.GetInfix() << endl;
    cout << expr.GetPostfix() << endl;
    vector<string> operands = expr.GetOperands();
    map<string, double> values;
    double val;
    for (auto value : operands) {
        cout << value << " ";
    }
    cout << "\n";
    for (const auto& op : operands) {
        cout << "Enter value of " << op << ": ";
        cin >> val;
        values[op] = val;
    }
    cout << expr.Calculate(values) << endl;

    return 0;
}
