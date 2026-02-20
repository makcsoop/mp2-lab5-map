#pragma once
#include <string.h>

#include <iostream>
#include <vector>

#include "TSimpleList.h"
using namespace std;

template <typename T, typename H>
class Map {
   public:
    struct Pair {
        T key;
        H value;

        bool operator<(const Pair& other) const { return key < other.key; }
    };
    Map() = default;
    ~Map() = default;
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
        int l = 0, r = data.size() - 1;
        while (l <= r) {
            int middle = l + (r - l) / 2;
            if (data[middle].key == key) {
                return &data[middle].value;
            }
            if (data[middle].key > key) {
                r = middle - 1;
            } else {
                l = middle + 1;
            }
        }
        return nullptr;
    }

    H& operator[](T key) {
        auto result = Find(key);
        if (result != nullptr) return *result;
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
        cout << "}\n";
    }

    void Insert(T key, H value) {
        if (Find(key) != nullptr) {
            return;
        }
        for (size_t i = 0; i < data.size(); i++) {
            if (data[i].key > key) {
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

// НЕУПОРЯДОЧННЫЕ ТАБЛИЦЫ НА СПИСКАХ
template <typename T, typename H>
class MapLists : public Map<T, H> {
   private:
    using typename Map<T, H>::Pair;
    TSimpleList<Pair> list;

   public:
    MapLists() {}
    ~MapLists() { list.~TSimpleList(); }
    void Insert(T key, H value) {
        if (Find(key) != nullptr) {
            return;
        }
        Pair newPair{key, value};
        list.PushFront(newPair);
    }
    H* Find(T key) {
        for (int i = 0; i < list.size(); i++) {
            if (list[i].key == key) {
                return &list[i].value;
            }
        }
        return nullptr;
    }
    int count() { return list.size(); }
    vector<T> keys() {
        vector<T> _keys;
        for (int i = 0; i < list.size(); i++) {
            _keys.push_back(list[i].key);
        }
        return _keys;
    }
    void Delete(T _key) {
        bool fl = 0;
        if (list.GetFirst()->value.key == _key) {
            list.PopFront();
            fl = 1;
        } else {
            for (int i = 0; i < list.size(); i++) {
                if (list[i].key == _key) {
                    list.EraseAfter(i - 1);
                    fl = 1;
                }
            }
        }
        if (fl == 0) {
            throw invalid_argument("Invalid argument");
        }
    }
    H& operator[](T key) {
        auto result = Find(key);
        if (result != nullptr)
            return *result;
        else {
            Pair newpair{key, H{}};
            list.PushFront(newpair);
            return list.GetFirst()->value.value;
        }
    }
};