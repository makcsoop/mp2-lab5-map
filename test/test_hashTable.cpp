#include <gtest/gtest.h>
#include "HashTable.h"


class OpenHashTableTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация перед каждым тестом
    }

    void TearDown() override {
        // Очистка после каждого теста
    }
};

// Тест 1: Конструктор и начальное состояние
TEST_F(OpenHashTableTest, ConstructorInitializesCorrectly) {
    OpenHashTable<std::string, int> table;
    
    EXPECT_EQ(table.count(), 0);
}

// Тест 2: Вставка одного элемента
TEST_F(OpenHashTableTest, InsertSingleElement) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    
    EXPECT_EQ(table.count(), 1);
    
    int* value = table.Find("key1");
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 100);
}

// Тест 3: Вставка нескольких элементов
TEST_F(OpenHashTableTest, InsertMultipleElements) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    table.Insert("key2", 200);
    table.Insert("key3", 300);
    
    EXPECT_EQ(table.count(), 3);
    
    int* value1 = table.Find("key1");
    int* value2 = table.Find("key2");
    int* value3 = table.Find("key3");
    
    ASSERT_NE(value1, nullptr);
    ASSERT_NE(value2, nullptr);
    ASSERT_NE(value3, nullptr);
    
    EXPECT_EQ(*value1, 100);
    EXPECT_EQ(*value2, 200);
    EXPECT_EQ(*value3, 300);
}

// Тест 4: Поиск несуществующего элемента
TEST_F(OpenHashTableTest, FindNonExistentKey) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    
    int* value = table.Find("key2");
    EXPECT_EQ(value, nullptr);
}

// Тест 5: Удаление элемента
TEST_F(OpenHashTableTest, DeleteElement) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    table.Insert("key2", 200);
    
    EXPECT_EQ(table.count(), 2);
    
    table.Delete("key1");
    
    EXPECT_EQ(table.count(), 1);
    
    int* value = table.Find("key1");
    EXPECT_EQ(value, nullptr);
    
    int* value2 = table.Find("key2");
    ASSERT_NE(value2, nullptr);
    EXPECT_EQ(*value2, 200);
}

// Тест 6: Удаление несуществующего элемента (должно выбросить исключение)
TEST_F(OpenHashTableTest, DeleteNonExistentThrowsException) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    
    EXPECT_THROW(table.Delete("key2"), std::invalid_argument);
}

// Тест 7: Вставка и поиск после удаления
TEST_F(OpenHashTableTest, InsertAfterDelete) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    table.Delete("key1");
    table.Insert("key2", 200);
    
    EXPECT_EQ(table.count(), 1);
    
    int* value1 = table.Find("key1");
    int* value2 = table.Find("key2");
    
    EXPECT_EQ(value1, nullptr);
    ASSERT_NE(value2, nullptr);
    EXPECT_EQ(*value2, 200);
}

// Тест 8: Перехеширование при заполнении > 87.5%
TEST_F(OpenHashTableTest, RehashWhenLoadFactorExceedsThreshold) {
    OpenHashTable<std::string, int> table;
    
    // Вставляем элементы, чтобы превысить порог 87.5%
    // Начальный размер 31, порог ~27 элементов
    for (int i = 0; i < 28; i++) {
        table.Insert("key" + std::to_string(i), i);
    }
    
    EXPECT_GT(table.count(), 27);
    
    // Проверяем, что все элементы доступны
    table.Print();
    for (int i = 0; i < 28; i++) {
        int* value = table.Find("key" + std::to_string(i));
        ASSERT_NE(value, nullptr);
        EXPECT_EQ(*value, i);
    }
    
}

// Тест 9: Обновление значения для существующего ключа
TEST_F(OpenHashTableTest, UpdateExistingKey) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("key1", 100);
    EXPECT_THROW(table.Insert("key1", 200), invalid_argument); // Вставка с тем же ключом
    
    EXPECT_EQ(table.count(), 1);
    
    int* value = table.Find("key1");
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 100);

    table["key1"] = 200;
    value = table.Find("key1");
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 200);

}

// Тест 10: Работа с разными типами данных (char*)
TEST_F(OpenHashTableTest, DifferentKeyTypes) {
    OpenHashTable<std::string, double> table;
    
    table.Insert("pi", 3.14159);
    table.Insert("e", 2.71828);
    table.Insert("sqrt2", 1.41421);
    
    double* pi = table.Find("pi");
    double* e = table.Find("e");
    double* sqrt2 = table.Find("sqrt2");
    
    ASSERT_NE(pi, nullptr);
    ASSERT_NE(e, nullptr);
    ASSERT_NE(sqrt2, nullptr);
    
    EXPECT_DOUBLE_EQ(*pi, 3.14159);
    EXPECT_DOUBLE_EQ(*e, 2.71828);
    EXPECT_DOUBLE_EQ(*sqrt2, 1.41421);
}

// Тест 11: Пустые строки в качестве ключей
TEST_F(OpenHashTableTest, EmptyStringKey) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("", 42);
    
    EXPECT_EQ(table.count(), 1);
    
    int* value = table.Find("");
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 42);
}

// Тест 12: Длинные строки в качестве ключей
TEST_F(OpenHashTableTest, LongStringKey) {
    OpenHashTable<std::string, std::string> table;
    
    std::string longKey(1000, 'a');
    table.Insert(longKey, "value");
    
    std::string* value = table.Find(longKey);
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, "value");
}

// Тест 13: Множественные удаления и вставки
TEST_F(OpenHashTableTest, MultipleDeletionsAndInsertions) {
    OpenHashTable<std::string, int> table;
    
    // Вставляем элементы
    for (int i = 0; i < 20; i++) {
        table.Insert("key" + std::to_string(i), i);
    }
    
    // Удаляем каждый второй
    for (int i = 0; i < 20; i += 2) {
        table.Delete("key" + std::to_string(i));
    }
    
    EXPECT_EQ(table.count(), 10);
    
    // Проверяем удаленные
    for (int i = 0; i < 20; i += 2) {
        int* value = table.Find("key" + std::to_string(i));
        EXPECT_EQ(value, nullptr);
    }
    
    // Проверяем оставшиеся
    for (int i = 1; i < 20; i += 2) {
        int* value = table.Find("key" + std::to_string(i));
        ASSERT_NE(value, nullptr);
        EXPECT_EQ(*value, i);
    }
    
    // Вставляем новые элементы
    for (int i = 100; i < 110; i++) {
        table.Insert("newkey" + std::to_string(i), i);
    }
    
    EXPECT_EQ(table.count(), 20);
}

// Тест 14: Проверка хеш-функции
TEST_F(OpenHashTableTest, HashFunctionConsistency) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("test", 123);
    
    // Тот же ключ должен давать тот же хеш и находить элемент
    int* value1 = table.Find("test");
    int* value2 = table.Find("test");
    
    ASSERT_NE(value1, nullptr);
    ASSERT_NE(value2, nullptr);
    EXPECT_EQ(value1, value2); // Указатели должны совпадать
    EXPECT_EQ(*value1, 123);
    EXPECT_EQ(*value2, 123);
}

// Тест 15: Отрицательные значения
TEST_F(OpenHashTableTest, NegativeValues) {
    OpenHashTable<std::string, int> table;
    
    table.Insert("negative", -100);
    table.Insert("zero", 0);
    table.Insert("positive", 100);
    
    int* neg = table.Find("negative");
    int* zero = table.Find("zero");
    int* pos = table.Find("positive");
    
    ASSERT_NE(neg, nullptr);
    ASSERT_NE(zero, nullptr);
    ASSERT_NE(pos, nullptr);
    
    EXPECT_EQ(*neg, -100);
    EXPECT_EQ(*zero, 0);
    EXPECT_EQ(*pos, 100);
}

// Тест 16: Очень большой объем данных
TEST_F(OpenHashTableTest, LargeDataSet) {
    OpenHashTable<std::string, int> table;
    
    const int NUM_ELEMENTS = 500;
    
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        table.Insert("key" + std::to_string(i), i);
    }
    
    EXPECT_EQ(table.count(), NUM_ELEMENTS);
    
    // Проверяем случайные элементы
    for (int i = 0; i < NUM_ELEMENTS; i += 50) {
        int* value = table.Find("key" + std::to_string(i));
        ASSERT_NE(value, nullptr);
        EXPECT_EQ(*value, i);
    }
}

// Тест 17: Специальные символы в ключах
TEST_F(OpenHashTableTest, SpecialCharactersInKeys) {
    OpenHashTable<std::string, std::string> table;
    
    table.Insert("!@#$%^&*()", "special1");
    table.Insert("key with spaces", "special2");
    table.Insert("\t\n\r", "special3");
    
    std::string* val1 = table.Find("!@#$%^&*()");
    std::string* val2 = table.Find("key with spaces");
    std::string* val3 = table.Find("\t\n\r");
    
    ASSERT_NE(val1, nullptr);
    ASSERT_NE(val2, nullptr);
    ASSERT_NE(val3, nullptr);
    
    EXPECT_EQ(*val1, "special1");
    EXPECT_EQ(*val2, "special2");
    EXPECT_EQ(*val3, "special3");
}

// Тест 18: Последовательные вставки и удаления
TEST_F(OpenHashTableTest, SequentialInsertDelete) {
    OpenHashTable<std::string, int> table;
    
    for (int i = 0; i < 50; i++) {
        table.Insert("key" + std::to_string(i), i);
        EXPECT_EQ(table.count(), i + 1);
        
        int* value = table.Find("key" + std::to_string(i));
        ASSERT_NE(value, nullptr);
        EXPECT_EQ(*value, i);
    }
    
    for (int i = 0; i < 50; i++) {
        table.Delete("key" + std::to_string(i));
        EXPECT_EQ(table.count(), 49 - i);
        
        int* value = table.Find("key" + std::to_string(i));
        EXPECT_EQ(value, nullptr);
    }
}

