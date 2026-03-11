#include <gtest/gtest.h>
#include <tree.h>
#include <random>
#include <string>
#include <vector>
#include <climits>

#include "log.h"
#include "map.h"

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
    EXPECT_EQ(table.isTrueSort(), true);
    table.Delete(70);
    table.printTree(table.GetFirst());
    EXPECT_EQ(table.isTrueSort(), true);
    EXPECT_EQ(table.count(), 8);
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
    int index = tree.count() - 1;
    auto x = tree.end();
    for (; index >= 0; index--) {
        EXPECT_EQ(x->data.key, el[index]);
        if (index > 0) --x;
    }
}

TEST_F(IteratorData, Last) {
    auto x = tree.end();
    EXPECT_EQ(x->data.key, 80);
}

TEST_F(IteratorData, First) {
    auto x = tree.begin();
    EXPECT_EQ(x->data.key, 10);
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
    int index = tree.count() - 2;
    --it;
    // vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};
    EXPECT_EQ(it[0].data.key, el[index]);
    EXPECT_EQ(it[-2].data.key, el[index - 2]);
    EXPECT_EQ((it - 3)->data.key, el[index - 3]);

    it -= 2;
    index -= 2;
    EXPECT_EQ(it->data.key, el[index]);

    it--;
    index--;
    EXPECT_EQ(it->data.key, el[index]);

    auto old = it--;
    int oldIndex = index--;
    EXPECT_EQ(old->data.key, el[oldIndex]);
    EXPECT_EQ(it->data.key, el[index]);
}

TEST_F(BasicTreeData, ConstructorFromVector) {
    vector<Map<int, char>::Pair> elements = {{50, 'A'}, {30, 'B'}, {70, 'C'}, {20, 'D'}, {40, 'E'},
                                             {60, 'F'}, {80, 'G'}, {10, 'H'}, {90, 'I'}};

    Tree<int, char> tree(elements);
    // tree.printTreeWithKey(tree.GetFirst());

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

TEST_F(BasicTreeData, SortFunc) { EXPECT_EQ(table.isTrueSort(), true); }

TEST(BasicTree, ConstructorFromPairVector) {
    vector<Map<int, string>::Pair> elements = {
        {5, "five"}, {3, "three"}, {7, "seven"}, {2, "two"}, {4, "four"}};
    Tree<int, string> tree(elements);

    EXPECT_EQ(tree.count(), 5);
    EXPECT_EQ(*tree.Find(5), "five");
    EXPECT_EQ(*tree.Find(3), "three");
    EXPECT_EQ(*tree.Find(7), "seven");
    EXPECT_EQ(*tree.Find(2), "two");
    EXPECT_EQ(*tree.Find(4), "four");
    EXPECT_TRUE(tree.isTrueSort());
}

TEST(BasicTree, ConstructorFromKeyVector) {
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    Tree<int, string> tree(keys);

    EXPECT_EQ(tree.count(), 7);
    EXPECT_EQ(tree.keys(), vector<int>({20, 30, 40, 50, 60, 70, 80}));
    EXPECT_TRUE(tree.isTrueSort());
}

TEST(BasicTree, ConstructorFromEmptyVector) {
    vector<Map<int, string>::Pair> empty_pairs;
    Tree<int, string> tree1(empty_pairs);
    EXPECT_EQ(tree1.count(), 0);
    EXPECT_EQ(tree1.GetFirst(), nullptr);

    vector<int> empty_keys;
    Tree<int, string> tree2(empty_keys);
    EXPECT_EQ(tree2.count(), 0);
    EXPECT_EQ(tree2.GetFirst(), nullptr);
}

TEST(BasicTree, CopyConstructor) {
    Tree<int, string> original;
    original.Insert(5, "five");
    original.Insert(3, "three");
    original.Insert(7, "seven");

    Tree<int, string> copy(original);  // Если есть конструктор копирования

    EXPECT_EQ(copy.count(), 3);
    EXPECT_EQ(*copy.Find(5), "five");
    EXPECT_EQ(*copy.Find(3), "three");
    EXPECT_EQ(*copy.Find(7), "seven");
}

TEST(BasicTree, MoveConstructor) {
    Tree<int, string> original;
    original.Insert(5, "five");
    original.Insert(3, "three");

    Tree<int, string> moved(std::move(original));

    EXPECT_EQ(moved.count(), 2);
    EXPECT_EQ(*moved.Find(5), "five");
    EXPECT_EQ(*moved.Find(3), "three");
}

// Тесты метода Insert
TEST_F(BasicTreeData, InsertDuplicateKey) {
    EXPECT_THROW(table.Insert(1, "duplicate"), invalid_argument);
    EXPECT_EQ(table.count(), 3);  // Размер не должен измениться
}

TEST_F(BasicTreeData, InsertMultipleValues) {
    table.Insert(4, "four");
    table.Insert(5, "five");
    table.Insert(0, "zero");

    EXPECT_EQ(table.count(), 6);
    EXPECT_EQ(table.keys(), vector<int>({0, 1, 2, 3, 4, 5}));
    EXPECT_EQ(*table.Find(4), "four");
    EXPECT_EQ(*table.Find(5), "five");
    EXPECT_EQ(*table.Find(0), "zero");
}

TEST_F(BasicTreeData, InsertLargeNumberOfElements) {
    for (int i = 100; i < 200; ++i) {
        table.Insert(i, "value" + to_string(i));
    }

    EXPECT_EQ(table.count(), 103);  // 3 исходных + 100 новых
    EXPECT_EQ(*table.Find(150), "value150");
}

// Тесты метода Find
TEST_F(BasicTreeData, FindNonExistentKey) {
    EXPECT_EQ(table.Find(100), nullptr);
    EXPECT_EQ(table.Find(-5), nullptr);
}

TEST_F(BasicTreeData, FindAfterInsert) {
    table.Insert(100, "hundred");
    auto* value = table.Find(100);
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, "hundred");
}

TEST_F(BasicTreeData, FindAfterDelete) {
    table.Insert(100, "hundred");
    table.Delete(100);
    EXPECT_EQ(table.Find(100), nullptr);
}

// Тесты метода FindNode
TEST_F(BasicTreeData, FindNodeExistent) {
    auto* node = table.FindNode(2);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data.key, 2);
    EXPECT_EQ(node->data.value, "one");
}

TEST_F(BasicTreeData, FindNodeNonExistent) { EXPECT_EQ(table.FindNode(100), nullptr); }

TEST_F(BasicTreeData, FindNodeCheckParent) {
    auto* node = table.FindNode(2);
    ASSERT_NE(node->parent, nullptr);
    EXPECT_EQ(node->parent->data.key, 1);  // Предполагая структуру дерева
}

// Тесты метода Delete
TEST_F(BasicTreeData, DeleteLeafNode) {
    table.Insert(4, "four");
    EXPECT_EQ(table.count(), 4);

    table.Delete(4);
    EXPECT_EQ(table.count(), 3);
    EXPECT_EQ(table.Find(4), nullptr);
    EXPECT_TRUE(table.isTrueSort());
}

TEST_F(BasicTreeData, DeleteNodeWithOneChild) {
    Tree<int, string> tree;
    tree.Insert(50, "fifty");
    tree.Insert(30, "thirty");
    tree.Insert(70, "seventy");
    tree.Insert(20, "twenty");
    // Узел 30 имеет только левого потомка 20

    EXPECT_TRUE(tree.isTrueSort());
    tree.Delete(30);

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(tree.Find(30), nullptr);
    EXPECT_NE(tree.Find(20), nullptr);
    EXPECT_TRUE(tree.isTrueSort());
}

TEST_F(BasicTreeData, DeleteNodeWithTwoChildren) {
    Tree<int, string> tree;
    tree.Insert(50, "fifty");
    tree.Insert(30, "thirty");
    tree.Insert(70, "seventy");
    tree.Insert(20, "twenty");
    tree.Insert(40, "forty");
    // Узел 50 имеет двух потомков

    EXPECT_TRUE(tree.isTrueSort());
    tree.Delete(50);

    EXPECT_EQ(tree.count(), 4);
    EXPECT_EQ(tree.Find(50), nullptr);
    EXPECT_NE(tree.Find(30), nullptr);
    EXPECT_NE(tree.Find(70), nullptr);
    EXPECT_TRUE(tree.isTrueSort());
}

TEST_F(BasicTreeData, DeleteRootNode) {
    Tree<int, string> tree;
    tree.Insert(50, "fifty");
    tree.Insert(30, "thirty");
    tree.Insert(70, "seventy");

    tree.Delete(50);

    EXPECT_EQ(tree.count(), 2);
    EXPECT_EQ(tree.Find(50), nullptr);
    EXPECT_TRUE(tree.isTrueSort());

    // Проверяем, что новый корень корректен
    auto* root = tree.GetFirst();
    ASSERT_NE(root, nullptr);
    EXPECT_TRUE(root->data.key == 30 || root->data.key == 70);
}

TEST_F(BasicTreeData, DeleteNonExistentKey) {
    EXPECT_THROW(table.Delete(100), runtime_error);  // или другой тип исключения
    EXPECT_EQ(table.count(), 3);
}

// Тесты метода count
TEST_F(BasicTreeData, CountAfterOperations) {
    EXPECT_EQ(table.count(), 3);

    table.Insert(4, "four");
    EXPECT_EQ(table.count(), 4);
    table.Delete(2);
    EXPECT_EQ(table.count(), 3);
    table.Delete(1);
    table.Delete(3);
    table.Delete(4);
    EXPECT_EQ(table.count(), 0);
}

// Тесты метода keys
TEST_F(BasicTreeData, KeysEmptyTree) {
    Tree<int, string> empty_tree;
    EXPECT_EQ(empty_tree.keys(), vector<int>());
}

TEST_F(BasicTreeData, KeysAfterDelete) {
    table.Insert(5, "five");
    table.Insert(0, "zero");
    table.printTree(table.GetFirst());
    table.Delete(2);
    table.printTree(table.GetFirst());

    EXPECT_EQ(table.keys(), vector<int>({0, 1, 3, 5}));
}

// Тесты метода isTrueSort
TEST_F(BasicTreeData, SortValidationOnValidTree) {
    EXPECT_TRUE(table.isTrueSort());

    table.Insert(10, "ten");
    table.Insert(6, "six");
    EXPECT_TRUE(table.isTrueSort());
}

// Тесты оператора []
TEST_F(BasicTreeData, BracketOperatorRead) {
    Tree<int, string> tree;
    tree.Insert(1, "one");

    EXPECT_EQ(tree[1], "one");
}

TEST_F(BasicTreeData, BracketOperatorWrite) {
    Tree<int, string> tree;
    tree[1] = "one";

    EXPECT_EQ(*tree.Find(1), "one");
    EXPECT_EQ(tree.count(), 1);
}

TEST_F(BasicTreeData, BracketOperatorChaining) {
    Tree<int, string> tree;
    tree[5] = "five";
    tree[5] = "FIVE";
    tree[3] = tree[5];  // Присваивание значения по ключу

    EXPECT_EQ(tree[5], "FIVE");
    EXPECT_EQ(tree[3], "FIVE");
}

// Тесты методов GetFirst и printTree (визуальная проверка)
TEST_F(BasicTreeData, GetFirstOnEmptyTree) {
    Tree<int, string> empty_tree;
    EXPECT_EQ(empty_tree.GetFirst(), nullptr);
}

TEST_F(BasicTreeData, GetFirstAfterOperations) {
    auto* root = table.GetFirst();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->data.key, 1);  // Предполагая, что 1 - корень

    table.Insert(0, "zero");
    root = table.GetFirst();
    EXPECT_EQ(root->data.key, 1);  // Корень не должен измениться

    table.Delete(1);
    root = table.GetFirst();
    EXPECT_NE(root->data.key, 1);  // Корень должен измениться
}

TEST_F(BasicTreeData, PrintTreeOutput) {
    // Визуальная проверка - выводим дерево
    testing::internal::CaptureStdout();
    table.printTree(table.GetFirst());
    string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тесты для работы с разными типами данных
TEST(BasicTree, DifferentTypes) {
    // Tree с ключами string
    Tree<string, int> string_tree;
    string_tree.Insert("apple", 5);
    string_tree.Insert("banana", 3);
    string_tree.Insert("cherry", 7);

    EXPECT_EQ(string_tree.count(), 3);
    EXPECT_EQ(*string_tree.Find("banana"), 3);
    EXPECT_TRUE(string_tree.isTrueSort());

    // Tree с ключами double
    Tree<double, string> double_tree;
    double_tree.Insert(3.14, "pi");
    double_tree.Insert(2.71, "e");
    double_tree.Insert(1.61, "phi");

    EXPECT_EQ(double_tree.count(), 3);
    EXPECT_EQ(*double_tree.Find(3.14), "pi");
}

// Тесты граничных случаев
// TEST(BasicTree, EdgeCases) {
//     Tree<int, string> tree;

//     // Минимальные и максимальные значения
//     tree.Insert(INT_MIN, "min");
//     tree.Insert(INT_MAX, "max");

//     EXPECT_EQ(*tree.Find(INT_MIN), "min");
//     EXPECT_EQ(*tree.Find(INT_MAX), "max");
//     EXPECT_TRUE(tree.isTrueSort());
// }

// Тесты итераторов - расширение существующих
TEST_F(IteratorData, IteratorBeginEndEquality) {
    auto begin = tree.begin();
    auto end = tree.end();
    EXPECT_NE(begin, end);

    Tree<int, string> empty_tree;
    // Для пустого дерева begin() должен равняться end()
    // Но нужно убедиться, что это реализовано
}

TEST_F(IteratorData, IteratorTraversal) {
    vector<int> traversed_keys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        traversed_keys.push_back(it->data.key);
    }
    traversed_keys.push_back(tree.end()->data.key);  // Добавляем последний элемент

    EXPECT_EQ(traversed_keys, el);
}

// Тесты на корректность структуры дерева после операций
TEST_F(BasicTreeData, TreeStructureAfterMultipleOperations) {
    Tree<int, string> tree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80, 10, 90};

    for (int k : keys) {
        tree.Insert(k, "value");
    }

    EXPECT_TRUE(tree.isTrueSort());
    tree.printTree(tree.GetFirst());
    // Удаляем узлы в разном порядке
    vector<int> delete_order = {20, 70, 50, 90};

    for (int k : delete_order) {
        tree.printTree(tree.GetFirst());
        tree.Delete(k);

        EXPECT_TRUE(tree.isTrueSort()) << "Tree became unsorted after deleting " << k;
    }
    tree.printTree(tree.GetFirst());
    // Проверяем оставшиеся ключи
    vector<int> remaining_keys = tree.keys();
    vector<int> expected_keys = {10, 30, 40, 60, 80};
    EXPECT_EQ(remaining_keys, expected_keys);
}

// Тест на работу с пользовательским типом данных
struct CustomType {
    int a;
    string b;

    bool operator<(const CustomType& other) const { return a < other.a; }
    bool operator>(const CustomType& other) const { return a > other.a; }
    bool operator<=(const CustomType& other) const { return a <= other.a; }
    bool operator>=(const CustomType& other) const { return a >= other.a; }
    bool operator==(const CustomType& other) const { return a == other.a; }
};

TEST(BasicTree, CustomKeyType) {
    Tree<CustomType, string> tree;
    CustomType key1{1, "one"};
    CustomType key2{2, "two"};
    CustomType key3{3, "three"};

    tree.Insert(key1, "value1");
    tree.Insert(key2, "value2");
    tree.Insert(key3, "value3");

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(key2), "value2");
    EXPECT_TRUE(tree.isTrueSort());
}

// TEST(AVLTreeTest, EmptyTree) {
//     AVLTree<int, string> tree;
//     EXPECT_EQ(tree.count(), 0);
//     EXPECT_EQ(tree.Find(5), nullptr);
// }
class AVLTreeTest : public ::testing::Test {
   protected:
    AVLTree<int, string> tree;
    void SetUp() override {
        tree.Insert(50, "fifty");
        tree.Insert(30, "thirty");
        tree.Insert(70, "seventy");
        tree.Insert(20, "twenty");
        tree.Insert(40, "forty");
        tree.Insert(60, "sixty");
        tree.Insert(80, "eighty");
    }
};

// Базовые тесты конструктора
TEST(AVLTree, DefaultConstructor) {
    AVLTree<int, string> tree;
    EXPECT_EQ(tree.count(), 0);
    EXPECT_EQ(tree.GetFirst(), nullptr);
}

TEST(AVLTree, CopyConstructor) {
    AVLTree<int, string> original;
    original.Insert(5, "five");
    original.Insert(3, "three");
    original.Insert(7, "seven");
    
    AVLTree<int, string> copy(original);
    
    EXPECT_EQ(copy.count(), 3);
    EXPECT_EQ(*copy.Find(5), "five");
    EXPECT_EQ(*copy.Find(3), "three");
    EXPECT_EQ(*copy.Find(7), "seven");
}

// Тесты вставки и балансировки
TEST(AVLTree, InsertSingleElement) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");
    
    EXPECT_EQ(tree.count(), 1);
    EXPECT_EQ(*tree.Find(10), "ten");
    
    // Проверка баланса (визуально через print)
    tree._printTreeWithKey(tree.GetFirst());
}

TEST(AVLTree, InsertMultipleElementsNoRotation) {
    AVLTree<int, string> tree;
    tree.Insert(50, "fifty");
    tree.Insert(30, "thirty");
    tree.Insert(70, "seventy");
    
    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(50), "fifty");
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(70), "seventy");
    
    tree._printTreeWithKey(tree.GetFirst());
}

TEST(AVLTree, InsertLeftLeftRotation) {
    AVLTree<int, string> tree;
    
    cout << "Insert 30..." << endl;
    tree.Insert(30, "thirty");
    cout << "After 30:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    cout << "\nInsert 20..." << endl;
    tree.Insert(20, "twenty");
    cout << "After 20:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    cout << "\nInsert 10..." << endl;
    tree.Insert(10, "ten");  // Здесь падает
    cout << "After 10:" << endl;
    tree._printTreeWithKey(tree.GetFirst());

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(20), "twenty");
    EXPECT_EQ(*tree.Find(10), "ten");
    
    EXPECT_EQ(tree.GetFirst()->data.key, 20);
}

TEST(AVLTree, InsertRightRightRotation) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");
    tree._printTreeWithKey(tree.GetFirst());
    tree.Insert(20, "twenty");
    tree._printTreeWithKey(tree.GetFirst());
    tree.Insert(30, "thirty");  // Должен вызвать RR rotation
    tree._printTreeWithKey(tree.GetFirst());
    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(20), "twenty");
    EXPECT_EQ(*tree.Find(30), "thirty");
    
    // После RR rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);
    tree._printTreeWithKey(tree.GetFirst());
}

TEST(AVLTree, InsertLeftRightRotation) {
    AVLTree<int, string> tree;
    tree.Insert(30, "thirty");
    tree.Insert(10, "ten");
    tree.Insert(20, "twenty");  // Должен вызвать LR rotation
    
    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(20), "twenty");
    
    // После LR rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);
    tree._printTreeWithKey(tree.GetFirst());
}

TEST(AVLTree, InsertRightLeftRotation) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");
    tree.Insert(30, "thirty");
    tree.Insert(20, "twenty");  // Должен вызвать RL rotation
    
    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(20), "twenty");
    
    // После RL rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);
    tree._printTreeWithKey(tree.GetFirst());
}

TEST(AVLTree, InsertComplexBalancing) {
    AVLTree<int, string> tree;
    vector<int> keys = {50, 40, 60, 30, 45, 55, 70, 20, 35, 47, 57, 65, 80};
    
    for (int key : keys) {
        tree.Insert(key, to_string(key));
    }
    
    EXPECT_EQ(tree.count(), static_cast<int>(keys.size()));
    
    // Проверяем, что все ключи найдены
    for (int key : keys) {
        EXPECT_EQ(*tree.Find(key), to_string(key));
    }
    
    // Визуально проверяем балансировку
    tree._printTreeWithKey(tree.GetFirst());
}

// Тесты удаления
TEST_F(AVLTreeTest, DeleteLeafNode) {

    tree._printTreeWithKey(tree.GetFirst());
    cout << "After deleting 20 (leaf):" << endl;
    tree.Delete(20);
    tree._printTreeWithKey(tree.GetFirst());
    
    EXPECT_EQ(tree.count(), 6);
    EXPECT_EQ(tree.Find(20), nullptr);
}

TEST_F(AVLTreeTest, DeleteNodeWithOneChild) {
    // Добавляем узел с одним ребенком для теста
    tree.Insert(25, "twenty-five");
    tree.Insert(22, "twenty-two");  // Создаем левого ребенка
    
    cout << "Before deletion:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    cout << "After deleting 25 (node with left child):" << endl;
    tree.Delete(25);
    tree._printTreeWithKey(tree.GetFirst());
    
    EXPECT_EQ(tree.count(), 8);
    EXPECT_EQ(tree.Find(25), nullptr);
    EXPECT_NE(tree.Find(22), nullptr);
}

TEST_F(AVLTreeTest, DeleteNodeWithTwoChildren) {
    cout << "Before deletion:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    cout << "After deleting 50 (root with two children):" << endl;
    tree.Delete(50);
    tree._printTreeWithKey(tree.GetFirst());
    
    EXPECT_EQ(tree.count(), 6);
    EXPECT_EQ(tree.Find(50), nullptr);
    
    // Проверяем, что структура сохранилась
    EXPECT_NE(tree.Find(30), nullptr);
    EXPECT_NE(tree.Find(70), nullptr);
}

TEST_F(AVLTreeTest, DeleteRootWithTwoChildren) {
    AVLTree<int, string> smallTree;
    smallTree.Insert(50, "fifty");
    smallTree.Insert(30, "thirty");
    smallTree.Insert(70, "seventy");
    
    cout << "Before deletion:" << endl;
    smallTree._printTreeWithKey(smallTree.GetFirst());
    
    cout << "After deleting root 50:" << endl;
    smallTree.Delete(50);
    smallTree._printTreeWithKey(smallTree.GetFirst());
    
    EXPECT_EQ(smallTree.count(), 2);
    EXPECT_EQ(smallTree.Find(50), nullptr);
}

TEST_F(AVLTreeTest, DeleteAndBalanceCheck) {
    AVLTree<int, string> balTree;
    balTree.Insert(50, "fifty");
    balTree.Insert(40, "forty");
    balTree.Insert(60, "sixty");
    balTree.Insert(30, "thirty");
    balTree.Insert(45, "forty-five");
    balTree.Insert(55, "fifty-five");
    balTree.Insert(70, "seventy");
    balTree.Insert(20, "twenty");
    balTree.Insert(35, "thirty-five");
    
    cout << "Before deletion:" << endl;
    balTree._printTreeWithKey(balTree.GetFirst());
    
    // Удаляем узел, который вызовет перебалансировку
    cout << "After deleting 60:" << endl;
    balTree.Delete(60);
    balTree._printTreeWithKey(balTree.GetFirst());
    
    // Проверяем, что все оставшиеся ключи доступны
    vector<int> remainingKeys = {50, 40, 30, 45, 55, 70, 20, 35};
    for (int key : remainingKeys) {
        EXPECT_NE(balTree.Find(key), nullptr);
    }
}

// TEST_F(AVLTreeTest, DeleteMultipleElements) {
//     vector<int> keys = {50, 30, 70, 20, 40, 60, 80, 75, 85};
    
//     // Создаем и заполняем дерево
//     AVLTree<int, string> mulTree;
//     for (int key : keys) {
//         mulTree.Insert(key, to_string(key));
//     }
    
//     cout << "Initial tree:" << endl;
//     mulTree._printTreeWithKey(mulTree.GetFirst());
    
//     // Удаляем элементы в определенном порядке
//     vector<int> deleteOrder = {20, 75, 50, 85};
//     for (int key : deleteOrder) {
//         cout << "After deleting " << key << ":" << endl;
//         mulTree.Delete(key);
//         mulTree._printTreeWithKey(mulTree.GetFirst());
        
//         // Проверяем, что удаленный ключ больше не существует
//         EXPECT_EQ(mulTree.Find(key), nullptr);
//     }
    
//     // Проверяем оставшиеся ключи
//     vector<int> expectedKeys = {30, 40, 60, 70, 80};
//     vector<int> remainingKeys = mulTree.keys();
//     sort(expectedKeys.begin(), expectedKeys.end());
//     sort(remainingKeys.begin(), remainingKeys.end());
//     EXPECT_EQ(remainingKeys, expectedKeys);
// }

TEST_F(AVLTreeTest, DeleteNonExistentKey) {
    EXPECT_NO_THROW(tree.Delete(999));  // Должно просто ничего не делать
    EXPECT_EQ(tree.count(), 7);
}

// Тесты Find и FindNode
TEST_F(AVLTreeTest, FindExistingKey) {
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(70), "seventy");
    EXPECT_EQ(*tree.Find(20), "twenty");
}

TEST_F(AVLTreeTest, FindNonExistingKey) {
    EXPECT_EQ(tree.Find(100), nullptr);
    EXPECT_EQ(tree.Find(55), nullptr);
}

TEST_F(AVLTreeTest, FindNodeExisting) {
    auto* node = tree.FindNode(40);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data.key, 40);
    EXPECT_EQ(node->data.value, "forty");
}

TEST_F(AVLTreeTest, FindNodeNonExisting) {
    EXPECT_EQ(tree.FindNode(999), nullptr);
}

// Тесты итераторов
TEST(AVLTree, IteratorBeginEnd) {
    AVLTree<int, string> tree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys) {
        tree.Insert(k, to_string(k));
    }
    
    auto begin = tree.begin();
    auto end = tree.end();
    
    EXPECT_NE(begin, end);
    EXPECT_EQ(begin->data.key, 20);  // Минимальный ключ
    EXPECT_EQ(end->data.key, 80);     // Максимальный ключ
}

TEST(AVLTree, IteratorTraversal) {
    AVLTree<int, string> tree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    vector<int> sortedKeys = {20, 30, 40, 50, 60, 70, 80};
    
    for (int k : keys) {
        tree.Insert(k, to_string(k));
    }
    
    vector<int> traversedKeys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        traversedKeys.push_back(it->data.key);
    }
    traversedKeys.push_back(tree.end()->data.key);  // Добавляем последний
    
    EXPECT_EQ(traversedKeys, sortedKeys);
}

TEST(AVLTree, IteratorPrefixIncrement) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");
    tree.Insert(5, "five");
    tree.Insert(15, "fifteen");
    
    auto it = tree.begin();
    EXPECT_EQ(it->data.key, 5);
    
    ++it;
    EXPECT_EQ(it->data.key, 10);
    
    ++it;
    EXPECT_EQ(it->data.key, 15);
}

TEST(AVLTree, IteratorPostfixIncrement) {
    AVLTree<int, string> tree;
    tree.Insert(10, "ten");
    tree.Insert(5, "five");
    
    auto it = tree.begin();
    auto old = it++;
    
    EXPECT_EQ(old->data.key, 5);
    EXPECT_EQ(it->data.key, 10);
}

// Тесты оператора []
TEST(AVLTree, BracketOperatorRead) {
    AVLTree<int, string> tree;
    tree.Insert(1, "one");
    tree.Insert(2, "two");
    
    EXPECT_EQ(tree[1], "one");
    EXPECT_EQ(tree[2], "two");
    EXPECT_EQ(tree[3], "");  // Несуществующий ключ создается с дефолтным значением
}

TEST(AVLTree, BracketOperatorWrite) {
    AVLTree<int, string> tree;
    tree[5] = "five";
    tree[3] = "three";
    
    EXPECT_EQ(tree.count(), 2);
    EXPECT_EQ(*tree.Find(5), "five");
    EXPECT_EQ(*tree.Find(3), "three");
    
    tree[5] = "FIVE";
    EXPECT_EQ(*tree.Find(5), "FIVE");
}

TEST(AVLTree, BracketOperatorChain) {
    AVLTree<int, string> tree;
    tree[5] = "five";
    tree[3] = tree[5];
    
    EXPECT_EQ(tree[3], "five");
}

// Тесты keys() и count()
// TEST_F(AVLTreeTest, KeysMethod) {
//     vector<int> keys = tree.keys();
//     vector<int> expectedKeys = {20, 30, 40, 50, 60, 70, 80};
    
//     sort(keys.begin(), keys.end());
//     EXPECT_EQ(keys, expectedKeys);
// }

// TEST_F(AVLTreeTest, KeysEmptyTree) {
//     AVLTree<int, string> emptyTree;
//     EXPECT_TRUE(emptyTree.keys().empty());
// }

TEST_F(AVLTreeTest, CountAfterOperations) {
    EXPECT_EQ(tree.count(), 7);
    
    tree.Insert(90, "ninety");
    EXPECT_EQ(tree.count(), 8);
    
    tree.Delete(30);
    EXPECT_EQ(tree.count(), 7);
    
    tree.Delete(20);
    tree.Delete(40);
    EXPECT_EQ(tree.count(), 5);
}

// Тесты на работу с разными типами данных
// TEST(AVLTree, StringKeys) {
//     AVLTree<string, int> tree;
//     tree.Insert("banana", 3);
//     tree.Insert("apple", 5);
//     tree.Insert("cherry", 7);
    
//     EXPECT_EQ(tree.count(), 3);
//     EXPECT_EQ(*tree.Find("apple"), 5);
//     EXPECT_EQ(*tree.Find("banana"), 3);
//     EXPECT_EQ(*tree.Find("cherry"), 7);
    
//     vector<string> keys = tree.keys();
//     vector<string> expectedKeys = {"apple", "banana", "cherry"};
//     EXPECT_EQ(keys, expectedKeys);
// }

TEST(AVLTree, DoubleKeys) {
    AVLTree<double, string> tree;
    tree.Insert(3.14, "pi");
    tree.Insert(2.71, "e");
    tree.Insert(1.61, "phi");
    
    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(3.14), "pi");
    EXPECT_EQ(*tree.Find(2.71), "e");
    EXPECT_EQ(*tree.Find(1.61), "phi");
}

// Тест на большое количество элементов (стресс-тест)
TEST(AVLTree, StressTest) {
    AVLTree<int, int> tree;
    const int NUM_ELEMENTS = 1000;
    
    // Вставляем много элементов
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        tree.Insert(i, i * 10);
        
    }
    logger("delete 1",1);
    EXPECT_EQ(tree.count(), NUM_ELEMENTS);
    
    // Проверяем, что все элементы на месте
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        ASSERT_NE(tree.Find(i), nullptr);
        EXPECT_EQ(*tree.Find(i), i * 10);
    }
    logger("finded 1",1);
    // Удаляем каждый второй элемент
    for (int i = 0; i < NUM_ELEMENTS; i += 2) {
        tree.Delete(i);
    }
    logger("delete 2",1);
    EXPECT_EQ(tree.count(), NUM_ELEMENTS / 2);
    
    // Проверяем удаленные
    for (int i = 1; i < NUM_ELEMENTS; i += 2) {
        EXPECT_NE(tree.Find(i), nullptr);
    }
    logger("find 2",1);
    for (int i = 0; i < NUM_ELEMENTS; i += 2) {
        EXPECT_EQ(tree.Find(i), nullptr);
    }
    logger("find 3",1);
}

// Тест случайных операций (сравнение с std::set)
TEST(AVLTree, RandomOperationsComparison) {
    AVLTree<int, int> avlTree;
    set<int> referenceSet;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000);
    uniform_int_distribution<> opDis(0, 2);
    
    const int NUM_OPERATIONS = 500;
    
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int key = dis(gen);
        int op = opDis(gen);
        
        if (op == 0) {  // Insert
            avlTree.Insert(key, key * 2);
            referenceSet.insert(key);
        } else if (op == 1) {  // Delete
            avlTree.Delete(key);
            referenceSet.erase(key);
        } else {  // Find
            auto avlFind = avlTree.Find(key);
            auto setFind = referenceSet.find(key);
            
            if (setFind == referenceSet.end()) {
                EXPECT_EQ(avlFind, nullptr);
            } else {
                EXPECT_NE(avlFind, nullptr);
            }
        }
        
        // Периодически проверяем размер
        if (i % 50 == 0) {
            EXPECT_EQ(avlTree.count(), static_cast<int>(referenceSet.size()));
        }
    }
}

// Тест на проверку свойств AVL дерева (балансировка)
TEST(AVLTree, AVLProperty) {
    AVLTree<int, string> tree;
    
    // Вставляем возрастающую последовательность (худший случай для обычного BST)
    for (int i = 1; i <= 100; ++i) {
        tree.Insert(i, to_string(i));
    }
    
    // Дерево должно оставаться сбалансированным
    // Мы не можем напрямую проверить высоту, но можем проверить,
    // что все операции поиска работают быстро (в тесте это просто проверка наличия)
    for (int i = 1; i <= 100; ++i) {
        EXPECT_NE(tree.Find(i), nullptr);
    }
}



// Тест на удаление корня после сложных вставок
TEST(AVLTree, DeleteRootAfterComplexInsert) {
    AVLTree<int, string> tree;
    vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 77, 90};
    
    for (int k : keys) {
        tree.Insert(k, to_string(k));
    }
    
    cout << "Complex tree before root deletion:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    tree.Delete(50);  // Удаляем корень
    
    cout << "After deleting root 50:" << endl;
    tree._printTreeWithKey(tree.GetFirst());
    
    // Проверяем, что все остальные ключи на месте
    for (int k : keys) {
        if (k != 50) {
            EXPECT_NE(tree.Find(k), nullptr);
        } else {
            EXPECT_EQ(tree.Find(50), nullptr);
        }
    }
}

// Тест на граничные значения
TEST(AVLTree, BoundaryValues) {
    AVLTree<int, string> tree;
    
    tree.Insert(INT_MIN, "min");
    tree.Insert(INT_MAX, "max");
    tree.Insert(0, "zero");
    
    EXPECT_EQ(*tree.Find(INT_MIN), "min");
    EXPECT_EQ(*tree.Find(INT_MAX), "max");
    EXPECT_EQ(*tree.Find(0), "zero");
    
    tree.Delete(INT_MIN);
    EXPECT_EQ(tree.Find(INT_MIN), nullptr);
    EXPECT_EQ(tree.count(), 2);
}

// Тест на вставку дубликатов
TEST(AVLTree, DuplicateInsert) {
    AVLTree<int, string> tree;
    tree.Insert(5, "five");
    
    EXPECT_THROW(tree.Insert(5, "another five"), invalid_argument);
    EXPECT_EQ(tree.count(), 1);
}

// Тест на пустое дерево
TEST(AVLTree, EmptyTreeOperations) {
    AVLTree<int, string> tree;
    
    EXPECT_EQ(tree.count(), 0);
    EXPECT_EQ(tree.Find(10), nullptr);
    //EXPECT_TRUE(tree.keys().empty());
    EXPECT_NO_THROW(tree.Delete(10));  // Удаление несуществующего не должно вызывать ошибку
}