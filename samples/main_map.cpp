#include "../include/map.h"
#include "string.h"
#include <iostream>

using namespace std;

int main(){
    TableArrSort<string, int> table;
    table["test"] = 1;
    table.Print();
    table["test"] = 2;
    table.Delete("test");
    table.Print();
    table.Insert("new_test", 3);
    table.Print();
    cout << *table.Find("new_test") << "\n";
    for (auto x:  table.keys()){
        cout << x;
    }

    cout << "SUCCESS";
}