#include <iostream>
#include <vector>

#include "../include/map.h"
#include "../include/tree.h"
#include "string.h"

using namespace std;

int main() {
    TableArrSort<string, int> table;
    table["test"] = 1;
    table.Print();
    table["test"] = 2;
    table.Delete("test");
    table.Print();
    table.Insert("new_test", 3);
    table.Insert("test1", 3);
    table.Print();
    cout << *table.Find("new_test") << "\n";
    for (auto x : table.keys()) {
        cout << x;
    }

    // Tree<int, string> tree;
    // tree.Insert(4, "gryasgf");
    // tree.Insert(1, "key");
    // tree.Insert(3, "gryasgf");
    // tree.Insert(2, "gryasgf");
    // tree.printTree(tree.Get_pFirst());

    cout << "SUCCESS";
}