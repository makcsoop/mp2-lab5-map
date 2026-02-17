#include <iostream>
#include <vector>

#include "../include/map.h"
#include "string.h"

using namespace std;

int main() {
    TableArrSort<string, int> table;
    MapLists<int, int> tab;
    tab[0] = 1;
    tab[1] = 2;
    tab[3] = 3;
    cout << tab.count();
    table["test"] = 1;
    table.Print();
    table["test"] = 2;
    table.Delete("test");
    table.Print();
    table.Insert("new_test", 3);
    table.Print();
    cout << *table.Find("new_test") << "\n";
    for (auto x : table.keys()) {
        cout << x;
    }

    cout << "SUCCESS";
}