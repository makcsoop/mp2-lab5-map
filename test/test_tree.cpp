#include <gtest/gtest.h>
#include <tree.h>

#include <random>
#include <set>
#include <string>
#include <vector>

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

// // Тесты граничных случаев
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

TEST_F(BasicTreeData, TestInterotEndMinus) {
    Tree<int, string> tree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80, 10, 90};

    for (int k : keys) {
        tree.Insert(k, "value");
    }

    EXPECT_TRUE(tree.isTrueSort());
    tree.printTree(tree.GetFirst());
    auto iterotorEnd = tree.end();
    EXPECT_EQ(90, (iterotorEnd--).m_ptr->data.key);
}

class RedBlackTreeTest : public ::testing::Test {
   protected:
    RedBlackTree<int, string> tree;

    void SetUp() override {
        logger("SetUp start", 1);
        tree.Insert(10, "ten");
        tree.Insert(20, "twenty");
        tree.Insert(30, "thirty");
        tree.Insert(15, "fifteen");
        tree.Insert(5, "five");
        tree.Insert(25, "twenty five");
        tree.Insert(35, "thirty five");
        logger("SetUp end", 1);
    }

    // Вспомогательная функция для проверки свойств красно-черного дерева
    bool validateRedBlackProperties(RedBlackTree<int, string>& rbTree) {
        auto root = rbTree.GetFirst();
        if (root == nullptr) return true;

        // Свойство 1: Корень черный
        if (root->color != 'b') {
            cout << "Root is not black!" << endl;
            return false;
        }

        // Проверка остальных свойств рекурсивно
        int blackHeight = -1;
        return validateNode(rbTree, root, 0, blackHeight);
    }

    bool validateNode(RedBlackTree<int, string>& rbTree, RBNode<int, string>* node, int currentBlackHeight,
                      int& expectedBlackHeight) {
        if (node == nullptr) {
            // Достигли NIL-листа
            if (expectedBlackHeight == -1) {
                expectedBlackHeight = currentBlackHeight;
            }
            return currentBlackHeight == expectedBlackHeight;
        }

        // Свойство 3: Красный узел не может иметь красного родителя
        if (node->color == 'r' && node->parent != nullptr && node->parent->color == 'r') {
            cout << "Red node " << node->data.key << " has red parent!" << endl;
            return false;
        }

        // Увеличиваем счетчик черных узлов
        int newBlackHeight = currentBlackHeight + (node->color == 'b' ? 1 : 0);

        // Рекурсивно проверяем левое и правое поддеревья
        return validateNode(rbTree, node->left, newBlackHeight, expectedBlackHeight) &&
               validateNode(rbTree, node->right, newBlackHeight, expectedBlackHeight);
    }

    // Вспомогательная функция для печати дерева с цветами
    void printTreeWithColors(RBNode<int, string>* root, string indent = "", bool isLeft = true) {
        if (root == nullptr) return;
        if (root->right) {
            printTreeWithColors(root->right, indent + (isLeft ? "│   " : "    "), false);
        }
        cout << indent << (isLeft ? "└── " : "┌── ") << root->data.key << "(" << root->color << ")" << endl;
        if (root->left) {
            printTreeWithColors(root->left, indent + (isLeft ? "    " : "│   "), true);
        }
    }
};

// Тест базовой вставки и свойств КЧ дерева
TEST_F(RedBlackTreeTest, BasicInsertionAndProperties) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");
    testTree.Insert(20, "twenty");
    testTree.Insert(30, "thirty");

    cout << "Tree after inserting 10,20,30:" << endl;
    testTree.printTree(testTree.GetFirst());

    // Проверяем, что дерево сбалансировалось
    auto root = testTree.GetFirst();
    EXPECT_TRUE(root->color == 'b');  // Корень должен быть черным

    // После вставки 10,20,30 ожидаем, что 20 станет корнем (из-за балансировки)
    EXPECT_EQ(root->data.key, 20);

    // Проверяем цвета
    if (root->left) {
        EXPECT_EQ(root->left->color, 'r');
    };
    if (root->right) {
        EXPECT_EQ(root->right->color, 'r');
    };

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест LL поворота
TEST_F(RedBlackTreeTest, LLRotation) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(30, "thirty");
    testTree.Insert(20, "twenty");
    testTree.Insert(10, "ten");  // Должен вызвать LL поворот

    cout << "Tree after LL rotation (insert 30,20,10):" << endl;
    testTree.printTree(testTree.GetFirst());

    auto root = testTree.GetFirst();
    EXPECT_EQ(root->data.key, 20);
    EXPECT_EQ(root->color, 'b');
    EXPECT_EQ(root->left->data.key, 10);
    EXPECT_EQ(root->left->color, 'r');
    EXPECT_EQ(root->right->data.key, 30);
    EXPECT_EQ(root->right->color, 'r');

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест RR поворота
TEST_F(RedBlackTreeTest, RRRotation) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");
    testTree.Insert(20, "twenty");
    testTree.Insert(30, "thirty");  // Должен вызвать RR поворот

    cout << "Tree after RR rotation (insert 10,20,30):" << endl;
    testTree.printTree(testTree.GetFirst());

    auto root = testTree.GetFirst();
    EXPECT_EQ(root->data.key, 20);
    EXPECT_EQ(root->color, 'b');
    EXPECT_EQ(root->left->data.key, 10);
    EXPECT_EQ(root->left->color, 'r');
    EXPECT_EQ(root->right->data.key, 30);
    EXPECT_EQ(root->right->color, 'r');

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест LR поворота
TEST_F(RedBlackTreeTest, LRRotation) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(30, "thirty");
    testTree.Insert(10, "ten");
    testTree.Insert(20, "twenty");  // Должен вызвать LR поворот

    cout << "Tree after LR rotation (insert 30,10,20):" << endl;
    testTree.printTree(testTree.GetFirst());

    auto root = testTree.GetFirst();
    EXPECT_EQ(root->data.key, 20);
    EXPECT_EQ(root->color, 'b');
    EXPECT_EQ(root->left->data.key, 10);
    EXPECT_EQ(root->left->color, 'r');
    EXPECT_EQ(root->right->data.key, 30);
    EXPECT_EQ(root->right->color, 'r');

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест RL поворота
TEST_F(RedBlackTreeTest, RLRotation) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");
    testTree.Insert(30, "thirty");
    testTree.Insert(20, "twenty");  // Должен вызвать RL поворот

    cout << "Tree after RL rotation (insert 10,30,20):" << endl;
    testTree.printTree(testTree.GetFirst());

    auto root = testTree.GetFirst();
    EXPECT_EQ(root->data.key, 20);
    EXPECT_EQ(root->color, 'b');
    EXPECT_EQ(root->left->data.key, 10);
    EXPECT_EQ(root->left->color, 'r');
    EXPECT_EQ(root->right->data.key, 30);
    EXPECT_EQ(root->right->color, 'r');

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест перекраски (случай с красным дядей)
TEST_F(RedBlackTreeTest, Recoloring) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");
    testTree.Insert(20, "twenty");
    testTree.Insert(30, "thirty");       // Вызовет RR поворот
    testTree.Insert(25, "twenty five");  // Должен вызвать перекраску

    cout << "Tree after recoloring case:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Вспомогательная функция для вычисления высоты дерева
int getTreeHeight(RBNode<int, string>* node) {
    if (node == nullptr) return 0;
    return 1 + max(getTreeHeight(node->left), getTreeHeight(node->right));
}

// Тест сложной вставки множества элементов
TEST_F(RedBlackTreeTest, ComplexInsertions) {
    RedBlackTree<int, string> testTree;
    vector<int> keys = {50, 25, 75, 15, 35, 60, 90, 10, 20, 30, 40, 55, 65, 80, 95};

    for (int key : keys) {
        testTree.Insert(key, "value" + to_string(key));
    }

    cout << "Complex tree after multiple insertions:" << endl;
    testTree.printTree(testTree.GetFirst());

    // Проверяем, что все ключи присутствуют
    for (int key : keys) {
        EXPECT_NE(testTree.Find(key), nullptr) << "Key " << key << " not found";
    }

    // Проверяем свойства КЧ дерева
    EXPECT_TRUE(validateRedBlackProperties(testTree));

    // Проверяем сортировку
    EXPECT_TRUE(testTree.isTrueSort());

    // Проверяем, что дерево сбалансировано (высота не более 2*log2(n+1))
    auto root = testTree.GetFirst();
    int height = getTreeHeight(root);
    int maxHeight = 2 * log2(keys.size() + 1) + 1;
    EXPECT_LE(height, maxHeight) << "Tree height " << height << " exceeds maximum " << maxHeight;
}

// Тест вставки с дубликатами
TEST_F(RedBlackTreeTest, DuplicateInsertion) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");

    EXPECT_THROW(testTree.Insert(10, "another ten"), invalid_argument);
    EXPECT_EQ(testTree.count(), 1);
    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест вставки в возрастающем порядке (худший случай для обычного дерева)
TEST_F(RedBlackTreeTest, AscendingInsertions) {
    RedBlackTree<int, string> testTree;

    for (int i = 1; i <= 100; i++) {
        testTree.Insert(i, "value" + to_string(i));
    }

    cout << "Tree height after 100 ascending insertions: " << getTreeHeight(testTree.GetFirst()) << endl;

    // Для 100 элементов идеальная высота сбалансированного дерева ~7
    // Красно-черное дерево должно иметь высоту не более 14
    int height = getTreeHeight(testTree.GetFirst());
    EXPECT_LE(height, 20) << "Tree too high, not properly balanced";

    EXPECT_TRUE(validateRedBlackProperties(testTree));

    // Проверяем, что все ключи на месте
    for (int i = 1; i <= 100; i++) {
        EXPECT_NE(testTree.Find(i), nullptr) << "Key " << i << " not found";
    }
}

// Тест вставки в убывающем порядке
TEST_F(RedBlackTreeTest, DescendingInsertions) {
    RedBlackTree<int, string> testTree;

    for (int i = 100; i >= 1; i--) {
        testTree.Insert(i, "value" + to_string(i));
    }

    cout << "Tree height after 100 descending insertions: " << getTreeHeight(testTree.GetFirst()) << endl;

    int height = getTreeHeight(testTree.GetFirst());
    EXPECT_LE(height, 20);

    EXPECT_TRUE(validateRedBlackProperties(testTree));

    for (int i = 1; i <= 100; i++) {
        EXPECT_NE(testTree.Find(i), nullptr) << "Key " << i << " not found";
    }
}

// // Тест случайных вставок
// TEST_F(RedBlackTreeTest, RandomInsertions) {
//     RedBlackTree<int, string> testTree;
//     set<int> insertedKeys;
//     mt19937 rng(40); // Фиксированный seed для воспроизводимости
//     uniform_int_distribution<int> dist(1, 200);

//     for (int i = 0; i < 100; i++) {
//         int key = dist(rng);
//         if (insertedKeys.find(key) == insertedKeys.end()) {
//             testTree.Insert(key, "value" + to_string(key));
//             insertedKeys.insert(key);
//         }
//     }

//     cout << "Tree height after " << insertedKeys.size() << " random insertions: "
//          << getTreeHeight(testTree.GetFirst()) << endl;

//     EXPECT_TRUE(validateRedBlackProperties(testTree));

//     // Проверяем, что все вставленные ключи присутствуют
//     for (int key : insertedKeys) {
//         EXPECT_NE(testTree.Find(key), nullptr) << "Key " << key << " not found";
//     }
// }

// Тест оператора [] с КЧ деревом
TEST_F(RedBlackTreeTest, BracketOperator) {
    RedBlackTree<int, string> testTree;

    testTree[5] = "five";
    testTree[3] = "three";
    testTree[7] = "seven";

    EXPECT_EQ(testTree[5], "five");
    EXPECT_EQ(testTree[3], "three");
    EXPECT_EQ(testTree[7], "seven");

    // Оператор [] должен создавать новые узлы при обращении к несуществующему ключу
    EXPECT_EQ(testTree[10], "");
    EXPECT_NE(testTree.Find(10), nullptr);

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест конструктора из вектора пар
TEST_F(RedBlackTreeTest, ConstructorFromVector) {
    // vector<Map<int, string>::Pair> elements = {{50, "fifty"}, {30, "thirty"}, {70, "seventy"}, {20,
    // "twenty"},
    //                                            {40, "forty"}, {60, "sixty"},  {80, "eighty"}};

    vector<int> el{10, 20, 30, 40, 50, 60, 65, 70, 80};

    RedBlackTree<int, string> testTree(el);

    cout << "Tree constructed from vector:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_EQ(testTree.count(), 9);
    EXPECT_TRUE(validateRedBlackProperties(testTree));

    for (const auto& elem : el) {
        EXPECT_NE(testTree.Find(elem), nullptr);
    }
}

// Тест итераторов для КЧ дерева
TEST_F(RedBlackTreeTest, IteratorTest) {
    RedBlackTree<int, string> testTree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    for (int key : keys) {
        testTree.Insert(key, "value" + to_string(key));
    }

    sort(keys.begin(), keys.end());

    // Проверка begin() и end()
    int index = 0;
    for (auto it = testTree.begin(); it != testTree.end(); ++it) {
        EXPECT_EQ(it->data.key, keys[index]);
        index++;
    }
    // Добавляем последний элемент
    EXPECT_EQ(testTree.end()->data.key, keys.back());

    // Проверка обратного прохода
    index = keys.size() - 1;
    auto it = testTree.end();
    while (index >= 0) {
        EXPECT_EQ(it->data.key, keys[index]);
        if (index > 0) --it;
        index--;
    }
}

// Тест удаления (когда будет реализовано)
TEST_F(RedBlackTreeTest, DeleteTest) {
    RedBlackTree<int, string> testTree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    for (int key : keys) {
        testTree.Insert(key, "value" + to_string(key));
    }
    string tmp;
    cout << "Before deletion:" << endl;
    testTree.printTree(testTree.GetFirst());

    // Удаление листа
    testTree.Delete(20);
    cout << "After deleting 20:" << endl;
    testTree.printTree(testTree.GetFirst());
    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_EQ(testTree.count(), 6);
    // cin >> tmp;
    // Удаление узла с одним потомком
    testTree.Delete(30);
    cout << "After deleting 30:" << endl;
    testTree.printTree(testTree.GetFirst());
    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_EQ(testTree.count(), 5);
    // cin >> tmp;
    // Удаление узла с двумя потомками
    // testTree.Delete(50);
    // cout << "After deleting 50:" << endl;
    // testTree.printTree(testTree.GetFirst());
    // EXPECT_TRUE(validateRedBlackProperties(testTree));
    // EXPECT_EQ(testTree.count(), 4);
    // cin >> tmp;
    // Удаление корня
    testTree.Delete(70);
    cout << "After deleting 70:" << endl;
    testTree.printTree(testTree.GetFirst());
    EXPECT_TRUE(validateRedBlackProperties(testTree));
}
// Тест удаления красного листа (Случай 0)
TEST_F(RedBlackTreeTest, DeleteRedLeaf) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");
    testTree.Insert(20, "twenty");
    testTree.Insert(30, "thirty");  // 30 станет красным листом

    cout << "Before deleting red leaf 30:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(30);

    cout << "After deleting red leaf 30:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_EQ(testTree.count(), 2);
    EXPECT_EQ(testTree.Find(30), nullptr);
}

// Тест удаления черного листа (сложный случай)
TEST_F(RedBlackTreeTest, DeleteBlackLeaf) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(20, "twenty");
    testTree.Insert(10, "ten");
    testTree.Insert(30, "thirty");

    cout << "Before deleting black leaf 10:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(10);

    cout << "After deleting black leaf 10:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_EQ(testTree.count(), 2);
}

// Тест удаления черного узла с красным ребенком (Случай 1)
TEST_F(RedBlackTreeTest, DeleteBlackWithRedChild) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(20, "twenty");  // корень
    testTree.Insert(10, "ten");     // станет красным

    cout << "Before deleting black root with red child:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(20);  // удаляем черный корень, у которого красный ребенок 10

    cout << "After deletion:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_EQ(testTree.GetFirst()->data.key, 10);
    EXPECT_EQ(testTree.GetFirst()->color, 'b');
}

// Тест удаления с красным братом (Случай 2A)
TEST_F(RedBlackTreeTest, DeleteWithRedSibling) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(50, "fifty");
    testTree.Insert(30, "thirty");
    testTree.Insert(70, "seventy");
    testTree.Insert(20, "twenty");
    testTree.Insert(40, "forty");

    cout << "Tree before deletion (should have red sibling case):" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(20);  // удаляем черный лист, должен активировать случай с красным братом

    cout << "After deleting 20:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест удаления с черным братом и черными племянниками (Случай 2B)
TEST_F(RedBlackTreeTest, DeleteWithBlackSiblingAndBlackNephews) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(50, "fifty");
    testTree.Insert(30, "thirty");
    testTree.Insert(70, "seventy");
    testTree.Insert(20, "twenty");
    testTree.Insert(40, "forty");
    testTree.Insert(60, "sixty");
    testTree.Insert(80, "eighty");

    cout << "Tree before deletion:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(20);  // удаляем черный лист

    cout << "After deleting 20:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест удаления с дальним красным племянником (Случай 2C)
TEST_F(RedBlackTreeTest, DeleteWithFarRedNephew) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(50, "fifty");
    testTree.Insert(30, "thirty");
    testTree.Insert(80, "eighty");
    testTree.Insert(20, "twenty");
    testTree.Insert(40, "forty");
    testTree.Insert(70, "seventy");
    testTree.Insert(90, "ninety");
    testTree.Insert(75, "seventy five");  // сделаем красным для случая с дальним племянником

    cout << "Tree before deletion:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(20);  // удаляем черный лист

    cout << "After deleting 20:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест удаления с ближним красным племянником (Случай 2D)
TEST_F(RedBlackTreeTest, DeleteWithCloseRedNephew) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(50, "fifty");
    testTree.Insert(30, "thirty");
    testTree.Insert(80, "eighty");
    testTree.Insert(20, "twenty");
    testTree.Insert(40, "forty");
    testTree.Insert(70, "seventy");
    testTree.Insert(90, "ninety");
    testTree.Insert(60, "sixty");  // сделаем красным для случая с ближним племянником

    cout << "Tree before deletion:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(90);  // удаляем черный лист

    cout << "After deleting 90:" << endl;
    testTree.printTree(testTree.GetFirst());

    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// Тест каскадного удаления (проблема поднимается вверх)
TEST_F(RedBlackTreeTest, CascadingDelete) {
    RedBlackTree<int, string> testTree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80, 10, 35, 45, 55, 65, 75, 85};

    for (int key : keys) {
        testTree.Insert(key, "value");
    }

    cout << "Initial tree:" << endl;
    testTree.printTree(testTree.GetFirst());

    // Удаляем в порядке, который вызовет несколько балансировок
    vector<int> deleteOrder = {10, 20, 35, 45, 55, 65};
    logger("CascadingDelete", 1);
    for (int key : deleteOrder) {
        cout << "\nDeleting " << key << ":" << endl;
        testTree.Delete(key);
        testTree.printTree(testTree.GetFirst());
        EXPECT_TRUE(validateRedBlackProperties(testTree));
    }
}

// Тест удаления корня с двумя детьми
TEST_F(RedBlackTreeTest, DeleteRootWithTwoChildren) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(50, "fifty");
    testTree.Insert(30, "thirty");
    testTree.Insert(70, "seventy");
    testTree.Insert(20, "twenty");
    testTree.Insert(40, "forty");
    testTree.Insert(60, "sixty");
    testTree.Insert(80, "eighty");

    cout << "Before deleting root 50:" << endl;
    testTree.printTree(testTree.GetFirst());

    testTree.Delete(50);

    cout << "After deleting root 50:" << endl;
    testTree.printTree(testTree.GetFirst());
    logger("DeleteRootWithTwoChildren", 1);

    EXPECT_TRUE(validateRedBlackProperties(testTree));
    EXPECT_NE(testTree.GetFirst()->data.key, 50);
}

// Тест удаления всех элементов по порядку
TEST_F(RedBlackTreeTest, DeleteAllElements) {
    RedBlackTree<int, string> testTree;
    vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    for (int key : keys) {
        testTree.Insert(key, "value");
    }

    sort(keys.begin(), keys.end());

    for (int key : keys) {
        cout << "\nDeleting " << key << ":" << endl;
        testTree.Delete(key);
        testTree.printTree(testTree.GetFirst());
        EXPECT_TRUE(validateRedBlackProperties(testTree));
    }

    EXPECT_EQ(testTree.count(), 0);
    EXPECT_EQ(testTree.GetFirst(), nullptr);
}

// Тест удаления в случайном порядке
// TEST_F(RedBlackTreeTest, RandomDeleteOrder) {
//     RedBlackTree<int, string> testTree;
//     vector<int> keys;

//     // Создаем дерево с 20 элементами
//     for (int i = 1; i <= 20; i++) {
//         testTree.Insert(i * 5, "value" + to_string(i * 5));
//         keys.push_back(i * 5);
//     }

//     cout << "Initial tree height: " << getTreeHeight(testTree.GetFirst()) << endl;
//     EXPECT_TRUE(validateRedBlackProperties(testTree));

//     // Перемешиваем ключи для удаления
//     shuffle(keys.begin(), keys.end(), mt19937(42));

//     // Удаляем в случайном порядке
//     for (size_t i = 0; i < keys.size(); i++) {
//         cout << "\nStep " << i + 1 << ": deleting " << keys[i] << endl;
//         testTree.Delete(keys[i]);
//         testTree.printTree(testTree.GetFirst());
//         EXPECT_TRUE(validateRedBlackProperties(testTree));
//     }
// }

// Тест удаления с проверкой инвариантов после каждой операции
TEST_F(RedBlackTreeTest, InvariantsAfterEachDelete) {
    RedBlackTree<int, string> testTree;

    // Строим большое дерево
    for (int i = 1; i <= 50; i++) {
        testTree.Insert(i, "value" + to_string(i));
    }

    vector<int> keys = testTree.keys();

    // Проверяем инварианты после каждого удаления
    for (int key : keys) {
        testTree.Delete(key);
        EXPECT_TRUE(validateRedBlackProperties(testTree)) << "Failed after deleting " << key;
        EXPECT_TRUE(testTree.isTrueSort()) << "BST property failed after deleting " << key;
    }
}

// Тест удаления и вставки (проверка стабильности)
TEST_F(RedBlackTreeTest, DeleteAndInsert) {
    RedBlackTree<int, string> testTree;

    for (int i = 1; i <= 10; i++) {
        testTree.Insert(i, "value" + to_string(i));
    }

    cout << "Initial tree:" << endl;
    testTree.printTree(testTree.GetFirst());

    // Удаляем и вставляем обратно
    testTree.Delete(5);
    cout << "\nAfter deleting 5:" << endl;
    testTree.printTree(testTree.GetFirst());
    EXPECT_TRUE(validateRedBlackProperties(testTree));

    testTree.Insert(5, "five again");
    cout << "\nAfter inserting 5 again:" << endl;
    testTree.printTree(testTree.GetFirst());
    EXPECT_TRUE(validateRedBlackProperties(testTree));

    // Проверяем, что все ключи на месте
    for (int i = 1; i <= 10; i++) {
        EXPECT_NE(testTree.Find(i), nullptr) << "Key " << i << " missing";
    }
}

// Тест удаления несуществующего ключа
TEST_F(RedBlackTreeTest, DeleteNonExistent) {
    RedBlackTree<int, string> testTree;
    testTree.Insert(10, "ten");

    EXPECT_THROW(testTree.Delete(20), runtime_error);
    EXPECT_EQ(testTree.count(), 1);
    EXPECT_TRUE(validateRedBlackProperties(testTree));
}

// // Стресс-тест с множеством операций удаления
// TEST_F(RedBlackTreeTest, DeleteStressTest) {
//     RedBlackTree<int, string> testTree;
//     const int NUM_KEYS = 100;

//     // Вставляем много ключей
//     for (int i = 1; i <= NUM_KEYS; i++) {
//         testTree.Insert(i, "value");
//     }

//     cout << "Initial tree height: " << getTreeHeight(testTree.GetFirst()) << endl;

//     // Удаляем половину ключей
//     for (int i = 1; i <= NUM_KEYS/2; i++) {
//         testTree.Delete(i * 2);  // удаляем четные
//         if (i % 10 == 0) {
//             cout << "After " << i << " deletions, height: " << getTreeHeight(testTree.GetFirst()) << endl;
//         }
//         EXPECT_TRUE(validateRedBlackProperties(testTree));
//     }

//     // Проверяем, что нечетные ключи остались
//     for (int i = 1; i <= NUM_KEYS; i++) {
//         if (i % 2 == 1) {
//             EXPECT_NE(testTree.Find(i), nullptr) << "Odd key " << i << " should exist";
//         } else {
//             EXPECT_EQ(testTree.Find(i), nullptr) << "Even key " << i << " should be deleted";
//         }
//     }
// }

// Тесты конструкторов
TEST(AVLTreeTest, DefaultConstructor) {
    AVLTree<int, std::string> tree;
    EXPECT_EQ(tree.count(), 0);
    EXPECT_EQ(tree.GetFirst(), nullptr);
}

TEST(AVLTreeTest, ConstructorWithKeyValue) {
    AVLTree<int, std::string> tree;
    tree.Insert(10, "ten");
    EXPECT_EQ(tree.count(), 1);
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(tree.GetFirst()->data.key, 10);
}

// TEST(AVLTreeTest, CopyConstructor) {
//     AVLTree<int, std::string> original;
//     original.Insert(5, "five");
//     original.Insert(3, "three");
//     original.Insert(7, "seven");

//     AVLTree<int, std::string> copy(original);

//     EXPECT_EQ(copy.count(), 3);
//     EXPECT_EQ(*copy.Find(5), "five");
//     EXPECT_EQ(*copy.Find(3), "three");
//     EXPECT_EQ(*copy.Find(7), "seven");
// }

// TEST(AVLTreeTest, ConstructorWithVectorPairs) {
//     std::vector<Map<int, std::string>::Pair> elements = {
//         {5, "five"}, {3, "three"}, {7, "seven"}, {1, "one"}, {9, "nine"}};

//     AVLTree<int, std::string> tree(elements);
//     EXPECT_EQ(tree.count(), 5);
//     EXPECT_EQ(*tree.Find(5), "five");
//     EXPECT_EQ(*tree.Find(3), "three");
//     EXPECT_EQ(*tree.Find(7), "seven");
//     EXPECT_EQ(*tree.Find(1), "one");
//     EXPECT_EQ(*tree.Find(9), "nine");
// }

// TEST(AVLTreeTest, ConstructorWithVectorKeys) {
//     std::vector<int> keys = {5, 3, 7, 1, 9};

//     AVLTree<int, std::string> tree(keys);
//     EXPECT_EQ(tree.count(), 5);
//     EXPECT_NE(tree.Find(5), nullptr);
//     EXPECT_NE(tree.Find(3), nullptr);
//     EXPECT_NE(tree.Find(7), nullptr);
//     EXPECT_NE(tree.Find(1), nullptr);
//     EXPECT_NE(tree.Find(9), nullptr);
// }

// Тесты вставки и балансировки
TEST(AVLTreeTest, InsertSingleElement) {
    AVLTree<int, std::string> tree;
    tree.Insert(10, "ten");

    EXPECT_EQ(tree.count(), 1);
    EXPECT_EQ(*tree.Find(10), "ten");
}

TEST(AVLTreeTest, InsertMultipleElementsNoRotation) {
    AVLTree<int, std::string> tree;

    tree.Insert(50, "fifty");
    cout << "1----\n";
    tree.printTree(tree.GetFirst());
    tree.Insert(30, "thirty");
    cout << "-2---\n";
    tree.Insert(70, "seventy");
    cout << "--3--\n";

    EXPECT_EQ(tree.count(), 3);
    cout << "--4--\n";
    EXPECT_EQ(*tree.Find(50), "fifty");
    cout << "----5\n";
    EXPECT_EQ(*tree.Find(30), "thirty");
    cout << "----6\n";
    EXPECT_EQ(*tree.Find(70), "seventy");
}

TEST(AVLTreeTest, InsertLeftLeftRotation) {
    AVLTree<int, std::string> tree;

    tree.Insert(30, "thirty");
    tree.Insert(20, "twenty");
    tree.Insert(10, "ten");  // LL вращение

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(20), "twenty");
    EXPECT_EQ(*tree.Find(10), "ten");

    // После LL rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);

    // Проверяем структуру
    auto* root = tree.GetFirst();
    EXPECT_EQ(root->data.key, 20);

    auto* left = static_cast<AVLNode<int, std::string>*>(root->left);
    auto* right = static_cast<AVLNode<int, std::string>*>(root->right);

    EXPECT_EQ(left->data.key, 10);
    EXPECT_EQ(right->data.key, 30);

    // Проверяем баланс
    EXPECT_EQ(root->_balance, 0);
    EXPECT_EQ(left->_balance, 0);
    EXPECT_EQ(right->_balance, 0);
}

TEST(AVLTreeTest, InsertRightRightRotation) {
    AVLTree<int, std::string> tree;

    tree.Insert(10, "ten");
    tree.Insert(20, "twenty");
    tree.Insert(30, "thirty");  // RR вращение

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(20), "twenty");
    EXPECT_EQ(*tree.Find(30), "thirty");

    // После RR rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);

    auto* root = tree.GetFirst();
    auto* left = static_cast<AVLNode<int, std::string>*>(root->left);
    auto* right = static_cast<AVLNode<int, std::string>*>(root->right);

    EXPECT_EQ(left->data.key, 10);
    EXPECT_EQ(right->data.key, 30);

    EXPECT_EQ(root->_balance, 0);
    EXPECT_EQ(left->_balance, 0);
    EXPECT_EQ(right->_balance, 0);
}

TEST(AVLTreeTest, InsertLeftRightRotation) {
    AVLTree<int, std::string> tree;

    tree.Insert(30, "thirty");
    tree.Insert(10, "ten");
    tree.Insert(20, "twenty");  // LR вращение

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(20), "twenty");

    // После LR rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);

    auto* root = tree.GetFirst();
    auto* left = static_cast<AVLNode<int, std::string>*>(root->left);
    auto* right = static_cast<AVLNode<int, std::string>*>(root->right);

    EXPECT_EQ(left->data.key, 10);
    EXPECT_EQ(right->data.key, 30);

    EXPECT_EQ(root->_balance, 0);
    EXPECT_EQ(left->_balance, 0);
    EXPECT_EQ(right->_balance, 0);
}

TEST(AVLTreeTest, InsertRightLeftRotation) {
    AVLTree<int, std::string> tree;

    tree.Insert(10, "ten");
    tree.Insert(30, "thirty");
    tree.Insert(20, "twenty");  // RL вращение

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(10), "ten");
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(20), "twenty");

    // После RL rotation корнем должно стать 20
    EXPECT_EQ(tree.GetFirst()->data.key, 20);

    auto* root = tree.GetFirst();
    auto* left = static_cast<AVLNode<int, std::string>*>(root->left);
    auto* right = static_cast<AVLNode<int, std::string>*>(root->right);

    EXPECT_EQ(left->data.key, 10);
    EXPECT_EQ(right->data.key, 30);

    EXPECT_EQ(root->_balance, 0);
    EXPECT_EQ(left->_balance, 0);
    EXPECT_EQ(right->_balance, 0);
}

TEST(AVLTreeTest, InsertComplexBalancing) {
    AVLTree<int, std::string> tree;
    std::vector<int> keys = {50, 40, 60, 30, 45, 55, 70, 20, 35, 47, 57, 65, 80};

    for (int key : keys) {
        tree.Insert(key, std::to_string(key));
    }

    EXPECT_EQ(tree.count(), static_cast<int>(keys.size()));

    // Проверяем, что все ключи найдены
    for (int key : keys) {
        EXPECT_EQ(*tree.Find(key), std::to_string(key));
    }

    // Проверяем, что дерево сбалансировано (высота примерно log2(n))
    auto* root = tree.GetFirst();
    EXPECT_NE(root, nullptr);
}

TEST(AVLTreeTest, InsertDuplicateThrows) {
    AVLTree<int, std::string> tree;
    tree.Insert(5, "five");

    EXPECT_THROW(tree.Insert(5, "another five"), std::invalid_argument);
    EXPECT_EQ(tree.count(), 1);
}

// Тесты удаления
class AVLTreeDeleteTest : public ::testing::Test {
   protected:
    AVLTree<int, std::string> tree;

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

TEST_F(AVLTreeDeleteTest, DeleteLeafNode) {
    EXPECT_EQ(tree.count(), 7);

    tree.Delete(20);  // лист

    EXPECT_EQ(tree.count(), 6);
    EXPECT_EQ(tree.Find(20), nullptr);

    // Проверяем, что остальные ключи существуют
    EXPECT_NE(tree.Find(50), nullptr);
    EXPECT_NE(tree.Find(30), nullptr);
    EXPECT_NE(tree.Find(70), nullptr);
    EXPECT_NE(tree.Find(40), nullptr);
    EXPECT_NE(tree.Find(60), nullptr);
    EXPECT_NE(tree.Find(80), nullptr);
}

TEST_F(AVLTreeDeleteTest, DeleteNodeWithOneChild) {
    tree.Insert(25, "twenty-five");
    tree.Insert(22, "twenty-two");  // создаем левого ребенка

    EXPECT_EQ(tree.count(), 9);

    tree.Delete(25);  // узел с одним ребенком

    EXPECT_EQ(tree.count(), 8);
    EXPECT_EQ(tree.Find(25), nullptr);
    EXPECT_NE(tree.Find(22), nullptr);
}

TEST_F(AVLTreeDeleteTest, DeleteNodeWithTwoChildren) {
    EXPECT_EQ(tree.count(), 7);

    tree.Delete(50);  // корень с двумя детьми

    EXPECT_EQ(tree.count(), 6);
    EXPECT_EQ(tree.Find(50), nullptr);

    // Проверяем, что структура сохранилась
    EXPECT_NE(tree.Find(30), nullptr);
    EXPECT_NE(tree.Find(70), nullptr);
    EXPECT_NE(tree.Find(20), nullptr);
    EXPECT_NE(tree.Find(40), nullptr);
    EXPECT_NE(tree.Find(60), nullptr);
    EXPECT_NE(tree.Find(80), nullptr);
}

TEST_F(AVLTreeDeleteTest, DeleteRootWithTwoChildren) {
    AVLTree<int, std::string> smallTree;
    smallTree.Insert(50, "fifty");
    smallTree.Insert(30, "thirty");
    smallTree.Insert(70, "seventy");

    EXPECT_EQ(smallTree.count(), 3);

    smallTree.Delete(50);  // корень с двумя детьми

    EXPECT_EQ(smallTree.count(), 2);
    EXPECT_EQ(smallTree.Find(50), nullptr);

    // Корнем должен стать 30 или 70 (в зависимости от реализации)
    auto* root = smallTree.GetFirst();
    EXPECT_TRUE(root->data.key == 30 || root->data.key == 70);
}

TEST_F(AVLTreeDeleteTest, DeleteAndBalanceCheck) {
    // Удаляем узел, который вызовет перебалансировку
    tree.Delete(20);

    // Проверяем, что все оставшиеся ключи доступны
    std::vector<int> remainingKeys = {50, 30, 70, 40, 60, 80};
    for (int key : remainingKeys) {
        EXPECT_NE(tree.Find(key), nullptr);
    }

    // Проверяем баланс корня
    auto* root = tree.GetFirst();
    auto* rootAVL = static_cast<AVLNode<int, std::string>*>(root);
    EXPECT_TRUE(rootAVL->_balance >= -1 && rootAVL->_balance <= 1);
}

TEST_F(AVLTreeDeleteTest, DeleteAllElements) {
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};

    for (int key : keys) {
        tree.Delete(key);
    }

    EXPECT_EQ(tree.count(), 0);
    EXPECT_EQ(tree.GetFirst(), nullptr);

    for (int key : keys) {
        EXPECT_EQ(tree.Find(key), nullptr);
    }
}

// Тесты Find и FindNode
TEST_F(AVLTreeDeleteTest, FindExistingKey) {
    EXPECT_EQ(*tree.Find(30), "thirty");
    EXPECT_EQ(*tree.Find(70), "seventy");
    EXPECT_EQ(*tree.Find(20), "twenty");
    EXPECT_EQ(*tree.Find(40), "forty");
}

TEST_F(AVLTreeDeleteTest, FindNonExistingKey) {
    EXPECT_EQ(tree.Find(100), nullptr);
    EXPECT_EQ(tree.Find(55), nullptr);
    EXPECT_EQ(tree.Find(15), nullptr);
}

TEST_F(AVLTreeDeleteTest, FindNodeExisting) {
    auto* node = tree.FindNode(40);
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->data.key, 40);
    EXPECT_EQ(node->data.value, "forty");

    auto* avlNode = static_cast<AVLNode<int, std::string>*>(node);
    EXPECT_EQ(avlNode->_balance, 0);
}

TEST_F(AVLTreeDeleteTest, FindNodeNonExisting) { EXPECT_EQ(tree.FindNode(999), nullptr); }

// Тесты оператора []
TEST(AVLTreeTest, BracketOperatorRead) {
    AVLTree<int, std::string> tree;
    tree.Insert(1, "one");
    tree.Insert(2, "two");

    EXPECT_EQ(tree[1], "one");
    EXPECT_EQ(tree[2], "two");
}

TEST(AVLTreeTest, BracketOperatorWrite) {
    AVLTree<int, std::string> tree;
    tree[5] = "five";
    tree[3] = "three";

    EXPECT_EQ(tree.count(), 2);
    EXPECT_EQ(*tree.Find(5), "five");
    EXPECT_EQ(*tree.Find(3), "three");

    tree[5] = "FIVE";
    EXPECT_EQ(*tree.Find(5), "FIVE");
}

TEST(AVLTreeTest, BracketOperatorChain) {
    AVLTree<int, std::string> tree;
    tree[5] = "five";
    tree[3] = tree[5];

    EXPECT_EQ(tree[3], "five");
}

TEST(AVLTreeTest, BracketOperatorCreatesDefault) {
    AVLTree<int, std::string> tree;

    // Обращение по несуществующему ключу должно создать элемент с дефолтным значением
    EXPECT_EQ(tree[10], "");
    EXPECT_EQ(tree.count(), 1);
    EXPECT_NE(tree.Find(10), nullptr);
}

// Тесты итераторов
TEST(AVLTreeTest, IteratorBeginEnd) {
    AVLTree<int, std::string> tree;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    for (int k : keys) {
        tree.Insert(k, std::to_string(k));
    }

    auto begin = tree.begin();
    auto end = tree.end();

    EXPECT_NE(begin, end);
    EXPECT_EQ(begin->data.key, 20);  // минимальный ключ
    EXPECT_EQ(end->data.key, 80);    // максимальный ключ
}

TEST(AVLTreeTest, IteratorTraversal) {
    AVLTree<int, std::string> tree;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    std::vector<int> sortedKeys = {20, 30, 40, 50, 60, 70, 80};

    for (int k : keys) {
        tree.Insert(k, std::to_string(k));
    }

    std::vector<int> traversedKeys;
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        traversedKeys.push_back(it->data.key);
    }
    traversedKeys.push_back(tree.end()->data.key);  // добавляем последний

    EXPECT_EQ(traversedKeys, sortedKeys);
}

TEST(AVLTreeTest, IteratorPrefixIncrement) {
    AVLTree<int, std::string> tree;
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

TEST(AVLTreeTest, IteratorPostfixIncrement) {
    AVLTree<int, std::string> tree;
    tree.Insert(10, "ten");
    tree.Insert(5, "five");

    auto it = tree.begin();
    auto old = it++;

    EXPECT_EQ(old->data.key, 5);
    EXPECT_EQ(it->data.key, 10);
}

TEST(AVLTreeTest, IteratorPrefixDecrement) {
    AVLTree<int, std::string> tree;
    tree.Insert(10, "ten");
    tree.Insert(5, "five");
    tree.Insert(15, "fifteen");

    auto it = tree.end();
    EXPECT_EQ(it->data.key, 15);

    --it;
    EXPECT_EQ(it->data.key, 10);

    --it;
    EXPECT_EQ(it->data.key, 5);
}

TEST(AVLTreeTest, IteratorEquality) {
    AVLTree<int, std::string> tree;
    tree.Insert(10, "ten");
    tree.Insert(5, "five");

    auto it1 = tree.begin();
    auto it2 = tree.begin();
    auto it3 = tree.end();

    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
    EXPECT_TRUE(it1 != it3);
}

// Тесты keys() и count()
TEST_F(AVLTreeDeleteTest, KeysMethod) {
    std::vector<int> keys = tree.keys();
    std::vector<int> expectedKeys = {20, 30, 40, 50, 60, 70, 80};

    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(keys, expectedKeys);
}

TEST_F(AVLTreeDeleteTest, KeysEmptyTree) {
    AVLTree<int, std::string> emptyTree;
    EXPECT_TRUE(emptyTree.keys().empty());
}

TEST_F(AVLTreeDeleteTest, CountAfterOperations) {
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
TEST(AVLTreeTest, StringKeys) {
    AVLTree<std::string, int> tree;
    tree.Insert("banana", 3);
    tree.Insert("apple", 5);
    tree.Insert("cherry", 7);

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find("apple"), 5);
    EXPECT_EQ(*tree.Find("banana"), 3);
    EXPECT_EQ(*tree.Find("cherry"), 7);

    std::vector<std::string> keys = tree.keys();
    std::vector<std::string> expectedKeys = {"apple", "banana", "cherry"};
    EXPECT_EQ(keys, expectedKeys);
}

TEST(AVLTreeTest, DoubleKeys) {
    AVLTree<double, std::string> tree;
    tree.Insert(3.14, "pi");
    tree.Insert(2.71, "e");
    tree.Insert(1.61, "phi");

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(3.14), "pi");
    EXPECT_EQ(*tree.Find(2.71), "e");
    EXPECT_EQ(*tree.Find(1.61), "phi");
}

TEST(AVLTreeTest, IntValues) {
    AVLTree<int, int> tree;
    tree.Insert(10, 100);
    tree.Insert(5, 50);
    tree.Insert(15, 150);

    EXPECT_EQ(tree.count(), 3);
    EXPECT_EQ(*tree.Find(10), 100);
    EXPECT_EQ(*tree.Find(5), 50);
    EXPECT_EQ(*tree.Find(15), 150);
}

// Стресс-тест
TEST(AVLTreeTest, StressTest) {
    AVLTree<int, int> tree;
    const int NUM_ELEMENTS = 1000;

    // Вставляем много элементов
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        tree.Insert(i, i * 10);
    }

    EXPECT_EQ(tree.count(), NUM_ELEMENTS);

    // Проверяем, что все элементы на месте
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        ASSERT_NE(tree.Find(i), nullptr);
        EXPECT_EQ(*tree.Find(i), i * 10);
    }

    // Удаляем каждый второй элемент
    for (int i = 0; i < NUM_ELEMENTS; i += 2) {
        tree.Delete(i);
    }

    EXPECT_EQ(tree.count(), NUM_ELEMENTS / 2);

    // Проверяем удаленные
    for (int i = 1; i < NUM_ELEMENTS; i += 2) {
        EXPECT_NE(tree.Find(i), nullptr);
    }

    for (int i = 0; i < NUM_ELEMENTS; i += 2) {
        EXPECT_EQ(tree.Find(i), nullptr);
    }
}

// Тест на удаление корня после сложных вставок
TEST(AVLTreeTest, DeleteRootAfterComplexInsert) {
    AVLTree<int, std::string> tree;
    std::vector<int> keys = {50, 25, 75, 10, 30, 60, 80, 5, 15, 27, 35, 55, 65, 77, 90};

    for (int k : keys) {
        tree.Insert(k, std::to_string(k));
    }

    tree.Delete(50);  // удаляем корень

    // Проверяем, что все остальные ключи на месте
    for (int k : keys) {
        if (k != 50) {
            EXPECT_NE(tree.Find(k), nullptr);
        } else {
            EXPECT_EQ(tree.Find(50), nullptr);
        }
    }

    // Проверяем баланс
    auto* root = static_cast<AVLNode<int, std::string>*>(tree.GetFirst());
    EXPECT_TRUE(root->_balance >= -1 && root->_balance <= 1);
}