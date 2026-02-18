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
