#include <iostream>
#include <vector>

#include "../include/HashTable.h"
#include "../include/map.h"
#include "string.h"

using namespace std;

int main() {
    OpenHashTable<string, int> table;
    // table.Insert("one", 1);
    // table.Insert("two", 2);
    // table.Print();
    // cout << table.count() << endl;
    // cout << *table.Find("one") << endl;
    // table.Delete("one");
    // table.Print();
    // cout << table.count() << endl;

    for (int i = 0; i < 1000; i++) {
        table.Insert(to_string(i), i);
        cout << i << endl;
    }
}
