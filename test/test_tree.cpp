#include <gtest/gtest.h>
#include <tree.h>
#include "map.h"
#include <string>
#include <vector>

using namespace std;

template <typename T>
class TableData : public ::testing::Test {
   protected:
    T table;
    void SetUp() override {
        table.Insert(1, "one");
        table.Insert(2, "one");
        table.Insert(3, "one");
    }
};

class IteratorData : public ::testing::Test {
   protected:
    vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};
    Tree<int, string> tree;
    IteratorData() : tree(el) {}
};

class BasicTreeData : public TableData<Tree<int, string>> {};

TEST(BasicTree, DefaultConstructor) { EXPECT_NO_THROW((Tree<int, string>())); }

TEST(BasicTree, KeyValueConstructor) { EXPECT_NO_THROW((Tree<int, string>(1, "one"))); }

TEST_F(BasicTreeData, KeyInsert) {
    table.Insert(4, "ine");
    EXPECT_EQ(table.count(), 4);
}

TEST_F(BasicTreeData, FindKey) {
    table.Insert(4, "ine");
    EXPECT_EQ(*table.Find(4), "ine");
    EXPECT_EQ(table.Find(5), nullptr);
}

TEST_F(BasicTreeData, PrintTree) {
    table.Insert(4, "ine");
    table.printTree(table.GetFirst());

    table.Insert(10, "ine");
    table.Insert(6, "ine");
    table.printTree(table.GetFirst());
}

TEST_F(BasicTreeData, AllKey) {
    table.Insert(4, "ine");
    table.Insert(10, "ine");
    table.Insert(6, "ine");
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3, 4, 6, 10}));
}

TEST_F(BasicTreeData, CorrectTrueSortKey) {
    table.Insert(4, "ine");
    table.Insert(10, "ine");
    table.Insert(6, "ine");
    EXPECT_EQ(table.GetFirst()->left, nullptr);
    EXPECT_EQ(table.GetFirst()->right->data.key, 2);
    EXPECT_EQ(table.GetFirst()->right->right->right->right->left->data.key, 6);
}

TEST_F(BasicTreeData, DeleteNodes) {
    table.Insert(10, "ine");
    table.Insert(6, "ine");
    table.printTree(table.GetFirst());
    table.Delete(6);
    EXPECT_EQ(table.count(), 4);
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3, 10}));
    table.printTree(table.GetFirst());
    table.Delete(2);
    EXPECT_EQ(table.count(), 3);
    EXPECT_EQ(table.keys(), vector<int>({1, 3, 10}));
    table.printTree(table.GetFirst());
}

TEST_F(BasicTreeData, DeleteNodes2) {
    table.Insert(10, "ine");
    table.Insert(6, "ine");
    table.printTree(table.GetFirst());
    table.Delete(10);
    table.printTree(table.GetFirst());
}

TEST(BasicTree, DeleteNodes3) {
    Tree<int, string> table;
    table.Insert(50, "fifty");
    table.Insert(30, "thirty");
    table.Insert(70, "seventy");
    table.Insert(20, "twenty");
    table.Insert(40, "forty");
    table.Insert(60, "sixty");
    table.Insert(80, "eighty");
    table.Insert(75, "seventy five");
    table.Insert(85, "eighty five");
    table.printTree(table.GetFirst());
    table.Delete(70);
    table.printTree(table.GetFirst());
    EXPECT_EQ(table.count(), 9);
    EXPECT_EQ(table.FindNode(70), nullptr);
    EXPECT_NE(table.FindNode(50), nullptr);
    EXPECT_NE(table.FindNode(30), nullptr);
    EXPECT_NE(table.FindNode(20), nullptr);
    EXPECT_NE(table.FindNode(40), nullptr);
    EXPECT_NE(table.FindNode(60), nullptr);
    EXPECT_NE(table.FindNode(80), nullptr);
    EXPECT_NE(table.FindNode(75), nullptr);
    EXPECT_NE(table.FindNode(85), nullptr);
}

TEST_F(IteratorData, RoundTree) {
    int index = 0;
    for (auto x : tree) {
        EXPECT_EQ(x.data.key, el[index]);
        index++;
    }
}

TEST_F(IteratorData, RoundMinus) {
    int index = tree.count()-1;
    auto x=tree.end();
    for (; index >= 0; index--) {
        EXPECT_EQ(x->data.key, el[index]);
        if (index > 0) --x;
    }
}

// TEST_F(IteratorData, LastPlusPlus) {
//     auto x=tree.end();
//     ++x;
//     EXPECT_EQ(x->data.key, 10 );
// }

TEST_F(IteratorData, Last) {
    auto x=tree.end();
    EXPECT_EQ(x->data.key, 80 );
}

// TEST_F(IteratorData, FirstMinusMinus) {
//     auto x=tree.begin();
//     --x;
//     EXPECT_EQ(x->data.key, 10 );
// }



TEST_F(IteratorData, First) {
    auto x=tree.begin();
    EXPECT_EQ(x->data.key, 10 );
}

TEST_F(IteratorData, ItIndexNode) {
    auto it = tree.begin();
    EXPECT_EQ(it[2].data.key, 30);
    EXPECT_EQ(it[0].data.key, 10);
    EXPECT_EQ(it[5].data.key, 60);
    EXPECT_EQ((it + 3)->data.key, 40);
    it += 5;
    EXPECT_EQ(it->data.key, 60);
    it++;
    EXPECT_EQ(it->data.key, 65);
}



TEST_F(IteratorData, ItIndexNodeReverse) {
    auto it = tree.end();
    int index=tree.count()-2;
    --it;
//vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};
    EXPECT_EQ(it[0].data.key, el[index]);
    EXPECT_EQ(it[-2].data.key, el[index-2]);
    EXPECT_EQ((it - 3)->data.key, el[index-3]);

    it -= 2;
    index-=2;
    EXPECT_EQ(it->data.key,el[index]);

    it--;
    index--;
    EXPECT_EQ(it->data.key, el[index]);

    auto old = it--;
    int oldIndex=index--;
    EXPECT_EQ(old->data.key, el[oldIndex]);
    EXPECT_EQ(it->data.key, el[index]);
}


TEST_F(BasicTreeData, ConstructorFromVector) {
    vector<Map<int,char>::Pair> elements = {
        {50,'A'}, {30,'B'}, {70,'C'}, {20,'D'}, {40,'E'},
        {60,'F'}, {80,'G'}, {10,'H'}, {90,'I'}
    };

    Tree<int,char> tree(elements);
    //tree.printTreeWithKey(tree.GetFirst());

    auto it = tree.FindNode(30);
    EXPECT_EQ(it->data.key, 30);
    EXPECT_EQ(it->data.value, 'B');

    it = tree.FindNode(80);
    EXPECT_EQ(it->data.key, 80);
    EXPECT_EQ(it->data.value, 'G');

    EXPECT_EQ(tree.begin()->data.key, 10);

    auto end = tree.end();
    EXPECT_EQ(end->data.key, 90);
}


TEST_F(BasicTreeData, OperatorBrackets) {
    Tree<int, string> tree;

    tree[5] = "five";
    tree[3] = "three";

    EXPECT_EQ(tree[5], "five");
    EXPECT_EQ(tree[3], "three");

    tree[5] = "FIVE";
    EXPECT_EQ(tree[5], "FIVE");

    EXPECT_EQ(tree[10], "");
    tree[10] = "ten";
    EXPECT_EQ(tree[10], "ten");
}