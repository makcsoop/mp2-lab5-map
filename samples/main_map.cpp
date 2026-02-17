#include <iostream>
#include <vector>

#include "../include/map.h"
#include "../include/tree.h"
#include "string.h"

using namespace std;

int main() {
    // TableArrSort<string, int> table;
    // table["test"] = 1;
    // table.Print();
    // table["test"] = 2;
    // table.Delete("test");
    // table.Print();
    // table.Insert("new_test", 3);
    // table.Insert("test1", 3);
    // table.Print();
    // cout << *table.Find("new_test") << "\n";
    // for (auto x : table.keys()) {
    //     cout << x;
    // }

    vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};
    Tree<int, string> tree(el);
    tree.printTree(tree.GetFirst());
    cout << tree.GetNext(tree.FindNode(80))->data.key;

    // cout << endl << tree.count();

    cout << endl << tree.FindNode(70)->data.key;

    // cout << tree.GetNext(tree.FindNode(80));
    cout << "SUCCESS";
}