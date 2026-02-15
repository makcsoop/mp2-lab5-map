#include <gtest/gtest.h>
#include <map.h>



// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc

TEST(TableArrSortTest, AddKeys) {
    TableArrSort<string, int> table;
    table["test"] = 1;
    EXPECT_EQ(table["test"], 1);
    table.Insert("gfsfsaf", 10);
    EXPECT_EQ(table["gfsfsaf"], 10);
}


TEST(TableArrSortTest, SortKey) {
    TableArrSort<int, string> table;
    table[1] = "one";
    table[3] = "three";
    table[2] = "two";
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3}));
}

TEST(TableArrSortTest, DeleteKey){
    TableArrSort<int, string> table;
    table[1] = "one";
    table[3] = "three";
    EXPECT_THROW(table.Delete(10), invalid_argument);
    EXPECT_NO_THROW(table.Delete(1));
}


TEST(TableArrSortTest, Count){
    TableArrSort<int, string> table;
    table[1] = "one";
    table[3] = "three";
    EXPECT_EQ(table.count(), 2);
    table[2] = "two";
    EXPECT_EQ(table.count(), 3);
}

TEST(TableArrSortTest, Keys){
    TableArrSort<int, string> table;
    table[1] = "one";
    table[3] = "three";
    EXPECT_EQ(table.keys(), vector<int>({1, 3}));
    table[2] = "two";
    EXPECT_EQ(table.keys(), vector<int>({1, 2, 3}));
}

TEST(TableArrSortTest, Find){
    TableArrSort<int, string> table;
    table[1] = "one";
    table[3] = "three";
    table[2] = "two";
    EXPECT_EQ(*table.Find(2), "two");
    EXPECT_EQ(table.Find(10), nullptr);
}
