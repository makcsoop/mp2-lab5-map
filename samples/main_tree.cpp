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
    cout << "SUCCESS\n";

    RedBlackTree<int, string> rdtree;
    cout << "First test\n";
    rdtree.Insert(10, "123");
    rdtree.printTree(rdtree.GetFirst());
    rdtree.Insert(15, "123");
    rdtree.printTree(rdtree.GetFirst());
    rdtree.Insert(5, "123");
    rdtree.printTree(rdtree.GetFirst());
    rdtree.Insert(3, "123");
    rdtree.printTree(rdtree.GetFirst());

    RedBlackTree<int, string> rdtree2;
    cout << "Two test\n";
    rdtree2.Insert(10, "123");
    rdtree2.printTree(rdtree2.GetFirst());
    rdtree2.Insert(15, "123");
    rdtree2.printTree(rdtree2.GetFirst());
    rdtree2.Insert(5, "123");
    rdtree2.printTree(rdtree2.GetFirst());
    rdtree2.Insert(2, "123");
    rdtree2.printTree(rdtree2.GetFirst());
    rdtree2.Insert(12, "123");
    rdtree2.printTree(rdtree2.GetFirst());
    rdtree2.Insert(3, "123");
    rdtree2.printTree(rdtree2.GetFirst());

    RedBlackTree<int, string> rdtree3;
    cout << "Three test LL\n";
    rdtree3.Insert(10, "123");
    rdtree3.printTree(rdtree3.GetFirst());
    rdtree3.Insert(5, "123");
    rdtree3.printTree(rdtree3.GetFirst());
    rdtree3.Insert(3, "123");
    rdtree3.printTree(rdtree3.GetFirst());

    RedBlackTree<int, string> rdtree4;
    cout << "Four test LR\n";
    rdtree4.Insert(10, "123");
    rdtree4.printTree(rdtree4.GetFirst());
    rdtree4.Insert(5, "123");
    rdtree4.printTree(rdtree4.GetFirst());
    rdtree4.Insert(7, "123");
    rdtree4.printTree(rdtree4.GetFirst());

    RedBlackTree<int, string> rdtree5;
    cout << "Four test RR\n";
    rdtree5.Insert(10, "123");
    rdtree5.printTree(rdtree5.GetFirst());
    rdtree5.Insert(20, "123");
    rdtree5.printTree(rdtree5.GetFirst());
    rdtree5.Insert(25, "123");
    rdtree5.printTree(rdtree5.GetFirst());

    RedBlackTree<int, string> rdtree6;
    cout << "Four test RL\n";
    rdtree6.Insert(10, "123");
    rdtree6.printTree(rdtree5.GetFirst());
    rdtree6.Insert(20, "123");
    rdtree6.printTree(rdtree5.GetFirst());
    rdtree6.Insert(25, "123");
    rdtree6.printTree(rdtree5.GetFirst());

    // rdtree.Insert(1, "1234");
    // rdtree.Insert(2, "1234");
    // rdtree.printTree(rdtree.GetFirst());
}