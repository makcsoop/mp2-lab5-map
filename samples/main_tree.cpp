#include <iostream>
#include <vector>

#include "../include/map.h"
#include "../include/tree.h"
#include "string.h"

using namespace std;
int main() {
    vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};
    Tree<int, string> tree(el);
    // tree.printTree(tree.GetFirst());
    // cout << "------" << endl;
    // cout << tree.GetNext(tree.FindNode(80))->data.key << endl;
    // // cout << endl << tree.count();

    // cout << tree.FindNode(70)->data.key << endl;
    // cout << tree.GetNext(tree.FindNode(80))->data.key << endl;
    tree.printTree(tree.GetFirst());
    for (auto i : tree) {
        cout << i.data.key << endl;
    }

    auto it = tree.begin();
    cout << it[2].data.key << endl;
    tree.printTreeWithKey(tree.GetFirst());
    cout << "SUCCESS";
}