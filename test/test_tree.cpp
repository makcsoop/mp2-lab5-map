#include <gtest/gtest.h>
#include <tree.h>

#include <string>

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
