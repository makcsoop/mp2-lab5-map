// Тесты для DHeap (d-ичная куча)

#include <gtest.h>

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "D-heap.h"
#include "log.h"

using namespace std;
// g++ test/*.cpp -I./include -I./gtest gtest/*.cc -fsanitize=address -fsanitize=leak

TEST(DHeapTest, DefaultConstructor) {
    DHeap<int> heap;
    EXPECT_NO_THROW(heap.Insert(1));
    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, VectorConstructor) {
    vector<int> data = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    DHeap<int> heap(data, 2);

    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, ConstructorWithDifferentD) {
    vector<int> data = {10, 5, 3, 8, 1, 9, 2};

    DHeap<int> heap2(data, 2);
    EXPECT_EQ(heap2.getMin(), 1);

    DHeap<int> heap3(data, 3);
    EXPECT_EQ(heap3.getMin(), 1);

    DHeap<int> heap4(data, 4);
    EXPECT_EQ(heap4.getMin(), 1);
}

TEST(DHeapTest, ConstructorEmptyVector) {
    vector<int> empty_data;
    EXPECT_THROW(DHeap<int> heap(empty_data, 2), std::invalid_argument);
}

TEST(DHeapTest, CopyConstructor) {
    vector<int> data = {5, 3, 8, 1};
    DHeap<int> original(data, 2);
    DHeap<int> copy(original);

    EXPECT_EQ(original.getMin(), copy.getMin());

    original.Insert(0);
    EXPECT_EQ(original.getMin(), 0);
    original.Print();
    copy.Print();
    EXPECT_EQ(copy.getMin(), 1);
}

TEST(DHeapTest, MoveConstructor) {
    vector<int> data = {5, 3, 8, 1};
    DHeap<int> original(data, 2);
    int original_min = original.getMin();

    DHeap<int> moved(std::move(original));
    EXPECT_EQ(moved.getMin(), original_min);
}

TEST(DHeapTest, CopyAssignment) {
    vector<int> data1 = {5, 3, 8, 1};
    vector<int> data2 = {10, 20, 30};

    DHeap<int> heap1(data1, 2);
    DHeap<int> heap2(data2, 2);

    heap2 = heap1;

    EXPECT_EQ(heap1.getMin(), heap2.getMin());

    heap1.Insert(0);
    EXPECT_EQ(heap1.getMin(), 0);
    EXPECT_EQ(heap2.getMin(), 1);
}

TEST(DHeapTest, MoveAssignment) {
    vector<int> data1 = {5, 3, 8, 1};
    vector<int> data2 = {10, 20, 30};

    DHeap<int> heap1(data1, 2);
    DHeap<int> heap2(data2, 2);
    int heap1_min = heap1.getMin();

    heap2 = std::move(heap1);
    EXPECT_EQ(heap2.getMin(), heap1_min);
}

TEST(DHeapTest, Insert) {
    DHeap<int> heap;

    heap.Insert(10);
    heap.Insert(5);
    heap.Insert(20);
    heap.Insert(3);
    heap.Insert(8);

    EXPECT_EQ(heap.getMin(), 3);

    heap.Insert(1);
    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, InsertMultipleValues) {
    DHeap<int> heap;

    for (int i = 100; i > 0; i--) {
        heap.Insert(i);
    }

    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, FindExistingKey) {
    vector<int> data = {5, 3, 8, 1, 9, 2};
    DHeap<int> heap(data, 2);

    int index = heap.find(3);
    EXPECT_GE(index, 0);
    EXPECT_LT(index, heap.size());

    index = heap.find(9);
    EXPECT_GE(index, 0);

    index = heap.find(1);
    EXPECT_EQ(index, 0);
}

TEST(DHeapTest, FindNonExistingKey) {
    vector<int> data = {5, 3, 8, 1, 9, 2};
    DHeap<int> heap(data, 2);

    EXPECT_THROW(heap.find(100), std::invalid_argument);
    EXPECT_THROW(heap.find(-1), std::invalid_argument);
}

TEST(DHeapTest, MinChildIndex) {
    vector<int> data = {1, 5, 3, 8, 2, 7, 4, 6, 9};
    DHeap<int> heap(data, 3);

    int min_child = heap.min_child_index(0);
    EXPECT_GE(min_child, 1);
    EXPECT_LE(min_child, 3);

    min_child = heap.min_child_index(1);
    EXPECT_GE(min_child, 4);
    EXPECT_LE(min_child, 6);
}

TEST(DHeapTest, DecreaseKeyBubbleUp) {
    vector<int> data = {10, 20, 15, 30, 25, 18};
    DHeap<int> heap(data, 2);

    heap.decreaseKey(30, 5);
    EXPECT_EQ(heap.getMin(), 5);

    heap.decreaseKey(20, 2);
    EXPECT_EQ(heap.getMin(), 2);
}

TEST(DHeapTest, DecreaseKeyBubbleDown) {
    vector<int> data = {5, 10, 8, 15, 12, 9};
    DHeap<int> heap(data, 2);

    heap.decreaseKey(15, 11);
    EXPECT_EQ(heap.getMin(), 5);
}

TEST(DHeapTest, DecreaseKeyNonExisting) {
    vector<int> data = {5, 3, 8, 1};
    DHeap<int> heap(data, 2);

    EXPECT_THROW(heap.decreaseKey(100, 0), std::invalid_argument);
}

TEST(DHeapTest, ExtractMin) {
    vector<int> data = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    DHeap<int> heap(data, 2);

    int min = heap.extractMin();
    EXPECT_EQ(min, 1);
    EXPECT_EQ(heap.getMin(), 2);

    min = heap.extractMin();
    EXPECT_EQ(min, 2);
    EXPECT_EQ(heap.getMin(), 3);
}

TEST(DHeapTest, ExtractMinUntilEmpty) {
    vector<int> data = {3, 1, 2};
    DHeap<int> heap(data, 2);

    heap.Print();

    EXPECT_EQ(heap.extractMin(), 1);
    heap.Print();
    EXPECT_EQ(heap.extractMin(), 2);
    EXPECT_EQ(heap.extractMin(), 3);
}

TEST(DHeapTest, ExtractMinSingleElement) {
    DHeap<int> heap;
    heap.Insert(42);

    EXPECT_EQ(heap.extractMin(), 42);
    EXPECT_TRUE(heap.IsEmpty());
}

TEST(DHeapTest, ExtractMinMaintainsHeapProperty) {
    vector<int> data = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    DHeap<int> heap(data, 2);

    vector<int> extracted;
    while (!heap.IsEmpty()) {
        extracted.push_back(heap.extractMin());
    }

    for (size_t i = 1; i < extracted.size(); i++) {
        EXPECT_LE(extracted[i - 1], extracted[i]);
    }
}

TEST(DHeapTest, GetMin) {
    vector<int> data = {5, 3, 8, 1};
    DHeap<int> heap(data, 2);

    EXPECT_EQ(heap.getMin(), 1);

    heap.Insert(0);
    EXPECT_EQ(heap.getMin(), 0);
}

TEST(DHeapTest, EmptyHeapOperations) {
    DHeap<int> heap;

    EXPECT_TRUE(heap.IsEmpty());
    EXPECT_THROW(heap.getMin(), std::exception);
    EXPECT_THROW(heap.extractMin(), std::exception);
    EXPECT_THROW(heap.find(1), std::invalid_argument);
}

TEST(DHeapTest, DuplicateValues) {
    vector<int> data = {5, 5, 3, 5, 8, 3, 1, 1};
    DHeap<int> heap(data, 2);

    EXPECT_EQ(heap.getMin(), 1);

    heap.extractMin();
    EXPECT_EQ(heap.getMin(), 1);

    heap.extractMin();
    EXPECT_EQ(heap.getMin(), 3);
}

TEST(DHeapTest, LargeDValue) {
    vector<int> data = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    DHeap<int> heap(data, 10);

    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, DEqualsOne) {
    vector<int> data = {5, 3, 8, 1, 9, 2};
    DHeap<int> heap(data, 1);

    EXPECT_NO_THROW(heap.getMin());
}

TEST(DHeapTest, PrintMethod) {
    vector<int> data = {1, 2, 3};
    DHeap<int> heap(data, 2);

    testing::internal::CaptureStdout();
    heap.Print();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_FALSE(output.empty());
    EXPECT_TRUE(output.find("1") != std::string::npos);
}

TEST(DHeapTest, ComplexSequence) {
    DHeap<int> heap;

    // Insert sequence
    for (int i = 50; i > 0; i--) {
        heap.Insert(i);
    }
    EXPECT_EQ(heap.getMin(), 1);

    // Extract some mins
    for (int i = 1; i <= 10; i++) {
        EXPECT_EQ(heap.extractMin(), i);
    }

    // Insert more values
    for (int i = 60; i <= 70; i++) {
        heap.Insert(i);
    }

    // Decrease key operations
    heap.decreaseKey(60, 5);
    EXPECT_EQ(heap.getMin(), 5);

    // Final extraction sequence
    int prev = heap.extractMin();
    while (!heap.IsEmpty()) {
        int current = heap.extractMin();
        EXPECT_LE(prev, current);
        prev = current;
    }
}

TEST(DHeapTest, StressTest) {
    DHeap<int> heap;
    const int NUM_ELEMENTS = 1000;

    // Insert many elements
    for (int i = NUM_ELEMENTS; i > 0; i--) {
        heap.Insert(i);
    }

    EXPECT_EQ(heap.getMin(), 1);

    // Extract all elements and verify sorted order
    int prev = heap.extractMin();
    for (int i = 1; i < NUM_ELEMENTS; i++) {
        int current = heap.extractMin();
        EXPECT_LE(prev, current);
        prev = current;
    }
}

TEST(DHeapTest, DifferentDataTypes) {
    // Test with double
    vector<double> double_data = {5.5, 3.3, 8.8, 1.1, 9.9};
    DHeap<double> double_heap(double_data, 2);
    EXPECT_DOUBLE_EQ(double_heap.getMin(), 1.1);

    // Test with string
    vector<string> string_data = {"banana", "apple", "cherry", "date"};
    DHeap<string> string_heap(string_data, 2);
    EXPECT_EQ(string_heap.getMin(), "apple");
}

TEST(DHeapTest, BoundaryIndices) {
    vector<int> data = {1, 100, 2, 99, 3, 98, 4, 97, 5};
    DHeap<int> heap(data, 3);

    // Verify heap property for all nodes
    for (int i = 0; i < heap.size(); i++) {
        for (int j = 1; j <= 3; j++) {
            int child_index = i * 3 + j;
            if (child_index < heap.size()) {
                EXPECT_LE(heap.getNode(i), heap.getNode(child_index));
            }
        }
    }
}

TEST(DHeapTest, DecreaseKeySameValue) {
    vector<int> data = {5, 3, 8, 1};
    DHeap<int> heap(data, 2);

    EXPECT_NO_THROW(heap.decreaseKey(3, 3));
    EXPECT_EQ(heap.getMin(), 1);
}

TEST(DHeapTest, ExtractMinFromEmptyAfterOperations) {
    DHeap<int> heap;
    heap.Insert(1);
    heap.Insert(2);
    heap.extractMin();
    heap.extractMin();

    EXPECT_TRUE(heap.IsEmpty());
    EXPECT_THROW(heap.extractMin(), std::exception);
    EXPECT_THROW(heap.getMin(), std::exception);
}