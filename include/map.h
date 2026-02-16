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
        cout << "{\n";
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


template <typename T, typename H>
class MapLists : public Map<T, H> {
   private:
    using typename Map<T, H>::Pair;

    struct TNode {
        Pair data;
        TNode* pNext;
        TNode(Pair p, TNode* next = nullptr) : data(p), pNext(next) {}
    };

    TNode* pFirst;
    int sz;

   public:
    MapLists() = default;
    ~MapLists(){
        TNode* current = pFirst;
        while(current!=nullptr){
            TNode* tmp = pFirst;
            pFirst = pFirst->pNext;
            delete tmp;
        }

    }
    void Insert(Pair<T key1, H value>) {
        if (Find(key1) != nullptr) {
            return 0;
        }
        Pair newPair{key1, value};
        pFirst = new TNode(newPair, pFirst);
        sz++;
    }
    H* Find(T key) {
        TNode* Current = pFirst;
        while (Current != nullptr) {
            if (Current->data.key == key) {
                return Current->data.value;
            }
            Current = Current->pNext;
        }
        pFirst = new TNode(Pair{key, H{}}, pFirst);
        sz++;
        return pFirst->data.value;
    }
    int count() { return sz; }
    vector<T> keys() {
        TNode* current = pFirst;
        vector<T> _keys;
        while (current != nullptr) {
            _keys.push_back(current->data.key);
            current = current->pNext;
        }
        return _keys;
    }
    void Delete(T _key) {
        if (pFirst == nullptr) return;
        if (pFirst->data.key == _key) {
            TNode* tmp = pFirst;
            pFirst = pFirst->pNext;
            delete tmp;
            sz--;
            return;
        }
        TNode* current = pFirst;
        while (current->pNext != nullptr) {
            if (current->pNext->data.key == _key) {
                TNode* tmp = current->pNext;
                current->pNext = current->pNext->pNext;
                delete tmp;
                sz--;
                return;
            }
            current = current->pNext;
        }
    }
};