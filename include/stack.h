// объявление и реализация шаблонного стека
// стек поддерживает операции:
// - вставка элемента,
// - извлечение элемента,
// - просмотр верхнего элемента (без удаления)
// - проверка на пустоту,
// - получение количества элементов в стеке
// - очистка стека
// при вставке в полный стек должна перевыделяться память

// проверка стека
// g++ test/test_main.cpp test/test_stack.cpp -I./include gtest/gtest-all.cc -I./gtest
#ifndef STACK_H
#define STACK_H

#define STEP 15
#include <algorithm>
#include <iostream>

template <typename T>
class Stake {
    T* data;
    int index;
    int capacity;

   public:
    Stake() : index(-1), capacity(10) { data = new T[capacity]; }

    Stake(int size) : index(-1), capacity(size) { data = new T[capacity]; }

    Stake(Stake& s) {
        capacity = s.capacity;
        data = new T[capacity];
        for (int i = 0; i < capacity; i++) {
            data[i] = s.data[i];
        }
        index = s.index;
    }

    Stake& operator=(const Stake& s) {
        if (this == &s) {
            return *this;
        }
        delete[] data;
        capacity = s.capacity;
        data = new T[capacity];
        for (int i = 0; i <= capacity; i++) {
            data[i] = s.data[i];
        }
        index = s.index;
        return *this;
    }

    void push(T v) {
        if (index + 1 >= capacity) {
            int new_capacity = capacity + STEP;
            T* new_data = new T[new_capacity];
            for (int i = 0; i <= index; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        data[++index] = v;
    }

    T pop() { return data[index--]; }

    bool is_empty() { return index == -1; }

    T top() { return data[index]; }

    int length() { return index + 1; }

    ~Stake() { delete[] data; }

    friend std::ostream& operator<<(std::ostream& ostr, const Stake& s) {
        ostr << "-----\n";
        for (int i = s.index; i >= 0; i--) {
            ostr << "| " << s.data[i] << " |\n";
        }
        if (s.index == -1) {
            ostr << "| " << " " << " |\n";
        }
        ostr << "-----\n";
        return ostr;
    }
};

#endif