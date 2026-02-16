// ����� ��� �����

#include <gtest.h>

#include <string>

#include "stack.h"

// Тест конструктора по умолчанию
TEST(StakeTest, DefaultConstructor) {
    Stake<int> stack;
    EXPECT_TRUE(stack.is_empty());
    EXPECT_EQ(stack.length(), 0);
}

// Тест конструктора с размером
TEST(StakeTest, ConstructorWithSize) {
    Stake<int> stack(20);
    EXPECT_TRUE(stack.is_empty());
    EXPECT_EQ(stack.length(), 0);
}

// Тест вставки и извлечения элементов
TEST(StakeTest, PushAndPop) {
    Stake<int> stack;

    stack.push(10);
    EXPECT_FALSE(stack.is_empty());
    EXPECT_EQ(stack.length(), 1);

    stack.push(20);
    stack.push(30);
    EXPECT_EQ(stack.length(), 3);

    EXPECT_EQ(stack.pop(), 30);
    EXPECT_EQ(stack.pop(), 20);
    EXPECT_EQ(stack.pop(), 10);
    EXPECT_TRUE(stack.is_empty());
}

// Тест просмотра верхнего элемента
TEST(StakeTest, TopElement) {
    Stake<int> stack;

    stack.push(42);
    EXPECT_EQ(stack.top(), 42);
    EXPECT_EQ(stack.length(), 1);  // top не должен удалять элемент

    stack.push(100);
    EXPECT_EQ(stack.top(), 100);
    EXPECT_EQ(stack.length(), 2);
}

// Тест с разными типами данных
TEST(StakeTest, DifferentDataTypes) {
    Stake<double> doubleStack;
    doubleStack.push(3.14);
    doubleStack.push(2.71);

    EXPECT_DOUBLE_EQ(doubleStack.pop(), 2.71);
    EXPECT_DOUBLE_EQ(doubleStack.top(), 3.14);

    Stake<std::string> stringStack;
    stringStack.push("hello");
    stringStack.push("world");

    EXPECT_EQ(stringStack.pop(), "world");
    EXPECT_EQ(stringStack.top(), "hello");
}

// Тест очистки стека через pop
TEST(StakeTest, ClearStackByPop) {
    Stake<int> stack;

    for (int i = 0; i < 5; ++i) {
        stack.push(i);
    }

    EXPECT_EQ(stack.length(), 5);
    EXPECT_FALSE(stack.is_empty());

    // Очищаем стек
    while (!stack.is_empty()) {
        stack.pop();
    }

    EXPECT_TRUE(stack.is_empty());
    EXPECT_EQ(stack.length(), 0);
}

// Тест на переполнение (должна перевыделяться память)
TEST(StakeTest, MemoryReallocation) {
    Stake<int> stack;

    // Добавляем больше элементов, чем начальная емкость (10)
    for (int i = 0; i < 25; ++i) {
        stack.push(i);
    }

    EXPECT_EQ(stack.length(), 25);

    // Проверяем, что элементы сохранились правильно
    for (int i = 24; i >= 0; --i) {
        EXPECT_EQ(stack.pop(), i);
    }

    EXPECT_TRUE(stack.is_empty());
}

// Тест копирующего конструктора
TEST(StakeTest, CopyConstructor) {
    Stake<int> original;
    original.push(1);
    original.push(2);
    original.push(3);

    Stake<int> copy = original;

    // Проверяем, что оба стека содержат одинаковые данные
    EXPECT_EQ(original.length(), copy.length());

    // Извлекаем элементы и проверяем их порядок
    EXPECT_EQ(original.pop(), 3);
    EXPECT_EQ(copy.pop(), 3);
    EXPECT_EQ(original.pop(), 2);
    EXPECT_EQ(copy.pop(), 2);
    EXPECT_EQ(original.pop(), 1);
    EXPECT_EQ(copy.pop(), 1);
}

// Тест присваивания
TEST(StakeTest, AssignmentOperator) {
    Stake<int> stack1;
    stack1.push(10);
    stack1.push(20);

    Stake<int> stack2;
    stack2.push(30);

    stack2 = stack1;  // Присваивание

    EXPECT_EQ(stack1.length(), stack2.length());
    EXPECT_EQ(stack1.pop(), 20);
    EXPECT_EQ(stack2.pop(), 20);
    EXPECT_EQ(stack1.pop(), 10);
    EXPECT_EQ(stack2.pop(), 10);
}

// Тест последовательных операций
TEST(StakeTest, SequentialOperations) {
    Stake<int> stack;

    // Серия push/pop операций
    stack.push(1);
    stack.push(2);
    EXPECT_EQ(stack.pop(), 2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    stack.push(4);
    EXPECT_EQ(stack.pop(), 4);
    EXPECT_EQ(stack.pop(), 3);
    EXPECT_EQ(stack.pop(), 1);
    EXPECT_TRUE(stack.is_empty());

    // Снова заполняем
    for (int i = 0; i < 10; ++i) {
        stack.push(i * 10);
    }

    EXPECT_EQ(stack.length(), 10);
    EXPECT_EQ(stack.top(), 90);
}

// Тест с большим количеством элементов
TEST(StakeTest, LargeNumberOfElements) {
    Stake<int> stack;
    const int COUNT = 1000;

    for (int i = 0; i < COUNT; ++i) {
        stack.push(i);
    }

    EXPECT_EQ(stack.length(), COUNT);

    for (int i = COUNT - 1; i >= 0; --i) {
        EXPECT_EQ(stack.pop(), i);
    }

    EXPECT_TRUE(stack.is_empty());
}

// Тест на пустой стек
TEST(StakeTest, EmptyStackOperations) {
    Stake<int> stack;

    EXPECT_TRUE(stack.is_empty());
    EXPECT_EQ(stack.length(), 0);

    // Поведение pop() и top() на пустом стеке зависит от вашей реализации
    // Если вы хотите добавить исключения, можно использовать:
    // EXPECT_THROW(stack.pop(), std::out_of_range);
}