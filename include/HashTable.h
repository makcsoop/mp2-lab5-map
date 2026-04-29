#include <iostream>

#include "log.h"
#include "map.h"

using namespace std;

template <typename T, typename H>
class OpenHashTable {  // перехеширование и проверка поиска после удаленного элемента и состояния у char
                       // тест бе  нчмарк гугл - фильтр кукушки
    struct Pair {
        T key;
        H value;

        bool operator<(const Pair &other) const { return key < other.key; }
    };
    Pair *data;
    unsigned char *state;

    int sz;
    int countNumber;

    int simpleNumbers[25] = {31,      67,      131,     257,      521,      1031,     2053,      4099,
                             8209,    16411,   32771,   65537,    131101,   262147,   524309,    1048583,
                             2097169, 4194319, 8388617, 16777259, 33554467, 67108879, 134217757, 268435459};
    int index_number = 0;

   public:
    OpenHashTable() {
        sz = simpleNumbers[0];
        countNumber = 0;
        data = new Pair[sz];
        state = new unsigned char[sz];
        for (int i = 0; i < sz; i++) state[i] = 0;
    }

    int size() { return sz; }

    int count(T key) {
        int res = 0;
        for (int i = 0; i < sz; i++) {
            if (state[i] == 1 && data[i].key == key) {
                res++;
            }
        }
        return res;
    }

    void Insert(T key, H value, bool flag_no_count = false) {
        int i = 0;
        int sum = 0;
        if ((double)countNumber / (double)sz > 0.875) {
            // logger("Перехеширование", 1);
            // logger("Заполнненость: " + to_string((double)countNumber / (double)sz), 1);
            // logger("sz: " + to_string(sz) + " countNumber: " + to_string(countNumber), 1);

            ReHash();
        }
        for (char ch : key) sum += ch;
        do {
            int index = hash(sum, i);
            if (data[index].key == key && state[index] == 1) {
                throw invalid_argument("Такой ключ уже есть");
            }
            if (state[index] != 1) {
                state[index] = 1;
                data[index] = Pair{key, value};
                if (!flag_no_count) {
                    countNumber++;
                }
                return;
            } else {
                i++;
            }
        } while (i != sz);
        // переполнение таблицы
        // ReHash();
        // Insert(key, value);
    }

    H *Find(T key) {
        int i = 0;
        int sum = 0;
        for (char ch : key) sum += ch;
        do {
            int index = hash(sum, i);
            if (state[index] == 0) {
                return nullptr;
            }
            if (state[index] == 1 && data[index].key == key) {
                return &data[index].value;
            }
            i++;
        } while (i != sz);

        return nullptr;
    }

    void Delete(T key) {
        int i = 0;
        int sum = 0;
        for (char ch : key) sum += ch;
        do {
            int index = hash(sum, i);
            if (state[index] == 0) {
                throw invalid_argument("Нет ключа в таблице");
            }
            if (data[index].key == key && state[index] == 1) {
                state[index] = 2;
                countNumber--;
                return;
            }
            i++;
        } while (i != sz);
        throw invalid_argument("Нет ключа в таблице");
    }

    int count() { return countNumber; }

    int hash(int sum, int i) { return (h1(sum) + i * h2(sum)) % sz; }

    int h1(int k) { return k % sz; }

    int h2(int k) { return 1 + k % (sz - 1); }

    void Print() {
        cout << "{\n";
        for (int i = 0; i < sz; i++) {
            if (state[i]) {
                cout << "\t" << data[i].key << ": " << data[i].value << endl;
            }
        }
        cout << "}\n";
    }

    void ReHash() {
        index_number++;
        Pair *new_data = new Pair[simpleNumbers[index_number]];
        Pair *all_number = new Pair[countNumber];
        int index = 0;
        for (int i = 0; i < sz; i++) {
            if (state[i] == 1) {
                all_number[index] = data[i];
                index++;
            }
        }
        sz = simpleNumbers[index_number];
        delete[] data;
        delete[] state;
        state = new unsigned char[sz];
        for (int i = 0; i < sz; i++) state[i] = 0;
        data = new_data;
        for (int i = 0; i < index; i++) {
            Insert(all_number[i].key, all_number[i].value, true);
        }
        delete[] all_number;
    }

    H &operator[](T key) {
        H *tmp = this->Find(key);
        if (tmp == nullptr) {
            this->Insert(key, H{});
            return *this->Find(key);
        }
        return *tmp;
    }
};