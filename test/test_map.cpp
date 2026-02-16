#include <gtest/gtest.h>
#include <map.h>

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc

class TableData : public ::testing::Test {
   protected:
    TableArrSort<int, string> table;
    void SetUp() override {
        table[1] = "one";
        table[3] = "three";
        table[2] = "two";
    }
};

TEST(TableArrSortTest, AddKeys) {
    TableArrSort<string, int> table;
    table["test"] = 1;
    EXPECT_EQ(table["test"], 1);
    table.Insert("gfsfsaf", 10);
    EXPECT_EQ(table["gfsfsaf"], 10);
}

TEST_F(TableData, SortKey) { EXPECT_EQ(table.keys(), vector<int>({1, 2, 3})); }

TEST_F(TableData, DeleteKey) {
    EXPECT_THROW(table.Delete(10), invalid_argument);
    EXPECT_NO_THROW(table.Delete(1));
}

TEST_F(TableData, Count) {
    EXPECT_EQ(table.count(), 3);
    table[4] = "two";
    EXPECT_EQ(table.count(), 4);
}

TEST_F(TableData, Keys) {
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3}));
    table[4] = "four";
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3, 4}));
}

TEST_F(TableData, Find) {
    EXPECT_EQ(*table.Find(2), "two");
    EXPECT_EQ(table.Find(10), nullptr);
}
