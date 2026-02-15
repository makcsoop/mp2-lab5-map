#pragma once
#include <string.h>

#include <iostream>
#include <vector>
using namespace std;

template <typename T, typename H>
class Map {
   public:
    struct Pair {
        T key;
        H value;
    };
    Map() = default;
    virtual ~Map() = default;
    virtual void Insert(T key, H value) = 0;
    virtual void Delete(T key) = 0;
    virtual int count() = 0;
    virtual vector<T> keys() = 0;
    virtual H* Find(T key) = 0;

    // void operator=(Map map){};
};

// УПОРЯДОЧНАЯ ТАБЛИЦА НА МАССИВЕ
template <typename T, typename H>
class TableArrSort : public Map<T, H> {
   private:
    using typename Map<T, H>::Pair;
    vector<Pair> data;

   public:
    H* Find(T key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].key == key) {
                return &data[i].value;
            }
        }
        return nullptr;
    }

    H& operator[](T key) {
        auto result = Find(key);
        if (result) return *result;
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].key > key) {
                data.insert(data.begin() + i, Pair{key, H{}});
                return data[i].value;
            }
        }
        data.push_back(Pair(key, H{}));
        return data[data.size() - 1].value;
    }
    void Print() {
        if (data.size() == 0) {
            cout << "{}\n";
            return;
        }
        cout << "{\n";
        for (size_t i = 0; i < data.size(); i++) {
            cout << "\t" << '"' << data[i].key << '"' << " : " << data[i].value << "";
            if (i != data.size() - 1) {
                cout << ",";
            }
            cout << "\n";
        }
        cout << "{\n";
    }

    void Insert(T key, H value) {
        if (Find(key)) {
            return;
        }
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].key < key) {
                data.insert(data.begin() + i, Pair{key, value});
                return;
            }
        }
        data.push_back(Pair{key, value});
    }

    void Delete(T key) {
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].key == key) {
                data.erase(data.begin() + i);
                return;
            }
        }
        throw invalid_argument("Key error");
    }

    int count() { return data.size(); }

    vector<T> keys() {
        vector<T> res;
        for (auto x : data) {
            res.push_back(x.key);
        }
        return res;
    }
    TableArrSort() = default;
    ~TableArrSort() = default;
};
