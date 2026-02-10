// ����� ��� ���������� �������������� ���������

#include <gtest.h>

#include <cmath>

#include "TSimpleList.h"

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc -fsanitize=address  -fsanitize=leak

TEST(TSimpleListTest, DefaultConstructor) {
    TSimpleList<int> list;
    EXPECT_TRUE(list.IsEmpty());
    EXPECT_EQ(list.size(), 0);
}

TEST(TSimpleListTest, VectorConstructor) {
    vector<int> vec = {1, 2, 3, 4, 5};
    TSimpleList<int> list(vec);

    EXPECT_FALSE(list.IsEmpty());
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[4], 5);
}

TEST(TSimpleListTest, ListConstructor) {
    vector<int> vec = {1, 2, 3, 4, 5};
    TSimpleList<int> list(vec);
    TSimpleList<int> list2(list);

    EXPECT_FALSE(list2.IsEmpty());
    EXPECT_EQ(list2.size(), 5);
    EXPECT_EQ(list2[0], 1);
    EXPECT_EQ(list2[1], 2);
    EXPECT_EQ(list2[4], 5);
}

TEST(TSimpleListTest, CopyConstructor) {
    vector<int> vec = {1, 2, 3};
    TSimpleList<int> original(vec);
    TSimpleList<int> copy(original);

    EXPECT_EQ(original.size(), copy.size());
    EXPECT_EQ(original[0], copy[0]);
    EXPECT_EQ(original[1], copy[1]);
    EXPECT_EQ(original[2], copy[2]);

    original.PushFront(0);
    EXPECT_EQ(original.size(), 4);
    EXPECT_EQ(copy.size(), 3);
}

TEST(TSimpleListTest, MoveConstructor) {
    vector<int> vec = {1, 2, 3};
    TSimpleList<int> original(vec);
    TSimpleList<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);

    EXPECT_TRUE(original.IsEmpty() || original.size() == 0);
}

TEST(TSimpleListTest, CopyAssignment) {
    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {4, 5, 6, 7};

    TSimpleList<int> list1(vec1);
    TSimpleList<int> list2(vec2);

    list2 = list1;

    EXPECT_EQ(list1.size(), list2.size());
    EXPECT_EQ(list1[0], list2[0]);
    EXPECT_EQ(list1[1], list2[1]);
    EXPECT_EQ(list1[2], list2[2]);

    list1.PushFront(0);
    EXPECT_EQ(list1.size(), 4);
    EXPECT_EQ(list2.size(), 3);
}

TEST(TSimpleListTest, MoveAssignment) {
    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {4, 5};

    TSimpleList<int> list1(vec1);
    TSimpleList<int> list2(vec2);

    list2 = std::move(list1);

    EXPECT_EQ(list2.size(), 3);
    EXPECT_EQ(list2[0], 1);
    EXPECT_EQ(list2[1], 2);
    EXPECT_EQ(list2[2], 3);

    EXPECT_NO_THROW(list1.IsEmpty());
}

TEST(TSimpleListTest, SelfAssignment) {
    vector<int> vec = {1, 2, 3};
    TSimpleList<int> list(vec);

    list = list;

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
}

TEST(TSimpleListTest, AccessMethods) {
    TSimpleList<int> list;

    list.PushFront(3);
    list.PushFront(2);
    list.PushFront(1);

    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);

    EXPECT_EQ(list.Front(), 1);

    list.Front() = 10;
    list[1] = 20;

    EXPECT_EQ(list[0], 10);
    EXPECT_EQ(list[1], 20);
}

TEST(TSimpleListTest, ConstAccess) {
    vector<int> vec = {1, 2, 3};
    const TSimpleList<int> list(vec);

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 2);
    EXPECT_EQ(list[2], 3);
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.IsEmpty());
}

TEST(TSimpleListTest, PopFront) {
    vector<int> vec = {1, 2, 3};
    TSimpleList<int> list(vec);

    list.PopFront();
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list[0], 2);

    list.PopFront();
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(list[0], 3);

    list.PopFront();
    EXPECT_TRUE(list.IsEmpty());
    EXPECT_EQ(list.size(), 0);
}

TEST(TSimpleListTest, PushAfter) {
    TSimpleList<int> list;
    list.PushFront(1);
    list.PushFront(2);
    list.PushFront(3);

    list.PushAfter(0, 10);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list[0], 3);
    EXPECT_EQ(list[1], 10);
    EXPECT_EQ(list[2], 2);
    EXPECT_EQ(list[3], 1);

    list.PushAfter(3, 20);
    EXPECT_EQ(list.size(), 5);
    EXPECT_EQ(list[4], 20);
}

TEST(TSimpleListTest, EraseAfter) {
    vector<int> vec = {1, 2, 3, 4, 5};
    TSimpleList<int> list(vec);

    list.EraseAfter(0);
    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 3);

    list.EraseAfter(2);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[1], 3);
    EXPECT_EQ(list[2], 4);
}

TEST(TSimpleListTest, BoundaryConditions) {
    TSimpleList<int> list;

    EXPECT_THROW(list[0], std::exception);

    list.PushFront(1);
    EXPECT_EQ(list[0], 1);
    EXPECT_NO_THROW(list.PopFront());

    EXPECT_TRUE(list.IsEmpty());
    EXPECT_THROW(list[0], std::exception);
}

TEST(TSimpleListTest, StreamOutput) {
    vector<int> vec = {1, 2, 3};
    TSimpleList<int> list(vec);

    stringstream ss;
    ss << list;

    string output = ss.str();
    EXPECT_TRUE(output.find("1") != string::npos);
    EXPECT_TRUE(output.find("2") != string::npos);
    EXPECT_TRUE(output.find("3") != string::npos);
}

TEST(TSimpleListTest, GetNode) {
    vector<int> vec = {1, 2, 3, 4, 5};
    TSimpleList<int> list(vec);

    EXPECT_EQ(list.GetNode(3)->value, 4);
    EXPECT_EQ(list.GetNode(4)->value, 5);
    EXPECT_EQ(list.GetNode(0)->value, 1);
    EXPECT_THROW(list.GetNode(100), std::exception);
}

TEST(TSimpleListTest, MemoryManagement) {
    for (int i = 0; i < 100; ++i) {
        TSimpleList<int> list;
        for (int j = 0; j < 100; ++j) {
            list.PushFront(j);
        }
    }

    TSimpleList<int> list;
    for (int i = 0; i < 1000; ++i) {
        list.PushFront(i);
    }
    for (int i = 0; i < 500; ++i) {
        list.PopFront();
    }
}
