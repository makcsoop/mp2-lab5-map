#pragma once

#include <iostream>
#include <utility>
#include <vector>

#include "map.h"
using namespace std;

template <typename T>
class TSimpleList {
   protected:
    struct TNode {
        T value;
        TNode* pNext;
    } typedef TNode;
    TNode* pFirst;
    int sz;

   public:
    TSimpleList() : pFirst(nullptr), sz(0) {};
    TSimpleList(const vector<T>& v);
    TSimpleList(const TSimpleList& list);
    TSimpleList(TSimpleList&& list) noexcept;
    TSimpleList& operator=(const TSimpleList& list);
    TSimpleList& operator=(TSimpleList&& list) noexcept;
    ~TSimpleList();
    int size() const noexcept;
    bool IsEmpty() const noexcept;
    T& Front() noexcept;
    void PushFront(const T& val);
    void PopFront() noexcept;
    T& operator[](int pos);
    const T& operator[](int pos) const;
    void PushAfter(int pos, const T& val);
    void EraseAfter(int pos);
    TNode* GetFirst();

    TNode* GetNode(int pos);

    friend ostream& operator<<(ostream& ost, const TSimpleList<T>& list) {
        TNode* tmp = list.pFirst;
        for (int i = 0; i < list.size(); i++) {
            ost << tmp->value << " ";
            tmp = tmp->pNext;
        }
        ost << "\n";
        return ost;
    }
};

template <typename T>
typename TSimpleList<T>::TNode* TSimpleList<T>::GetFirst() {
    return pFirst;
}

template <typename T>
TSimpleList<T>::TSimpleList(const vector<T>& v) {
    pFirst = nullptr;
    sz = 0;
    if (v.size() > 0) {
        TNode* tmp = new TNode{v[0], nullptr};
        pFirst = tmp;
        sz++;
        for (int i = 1; i < v.size(); i++) {
            TNode* tmp1 = new TNode{v[i], nullptr};
            tmp->pNext = tmp1;
            tmp = tmp1;
            sz++;
        }
    }
}

template <typename T>
TSimpleList<T>::TSimpleList(const TSimpleList& list) {
    pFirst = nullptr;
    sz = 0;

    if (list.pFirst == nullptr) {
        return;
    }

    pFirst = new TNode{list.pFirst->value, nullptr};
    sz = 1;

    TNode* this_node = pFirst;
    TNode* list_node = list.pFirst->pNext;

    while (list_node != nullptr) {
        TNode* newNode = new TNode{list_node->value, nullptr};
        this_node->pNext = newNode;
        this_node = newNode;
        list_node = list_node->pNext;
        sz++;
    }
}

template <typename T>
TSimpleList<T>::TSimpleList(TSimpleList&& list) noexcept : pFirst(list.pFirst), sz(list.sz) {
    list.pFirst = nullptr;
    list.sz = 0;
}

template <typename T>
TSimpleList<T>& TSimpleList<T>::operator=(const TSimpleList& list) {
    if (this == &list) {
        return *this;
    }

    TNode* oldFirst = pFirst;
    int oldSize = sz;

    pFirst = new TNode{list.pFirst->value, nullptr};
    sz = list.sz;

    TNode* this_node = pFirst;
    TNode* list_node = list.pFirst->pNext;

    while (list_node != nullptr) {
        this_node->pNext = new TNode{list_node->value, nullptr};
        this_node = this_node->pNext;
        list_node = list_node->pNext;
    }
    while (oldFirst != nullptr) {
        TNode* tmp = oldFirst;
        oldFirst = oldFirst->pNext;
        delete tmp;
    }

    return *this;
}

template <typename T>
TSimpleList<T>& TSimpleList<T>::operator=(TSimpleList&& list) noexcept {
    pFirst = list.pFirst;
    sz = list.sz;
    list.pFirst = nullptr;
    list.sz = 0;
    return *this;
}

template <typename T>
TSimpleList<T>::~TSimpleList() {
    TNode* current = pFirst;
    while (current != nullptr) {
        TNode* next = current->pNext;
        delete current;
        current = next;
    }
    pFirst = nullptr;
    sz = 0;
}

template <typename T>
T& TSimpleList<T>::operator[](int pos) {
    if (pos < 0 || pos > sz - 1) {
        throw out_of_range("Error range");
    } else {
        TNode* tmp = pFirst;
        for (int i = 0; i < sz; i++) {
            if (pos == i) {
                return tmp->value;
            }
            tmp = tmp->pNext;
        }
        throw invalid_argument("Not found value");
    }
}

template <typename T>
const T& TSimpleList<T>::operator[](int pos) const {
    if (pos < 0 || pos > sz - 1) {
        throw out_of_range("Error range");
    } else {
        TNode* tmp = pFirst;
        for (int i = 0; i < sz; i++) {
            if (pos == i) {
                return tmp->value;
            }
            tmp = tmp->pNext;
        }
        throw invalid_argument("Not found value");
    }
}

template <typename T>
int TSimpleList<T>::size() const noexcept {
    return sz;
}

template <typename T>
bool TSimpleList<T>::IsEmpty() const noexcept {
    return sz == 0;
}

template <typename T>
T& TSimpleList<T>::Front() noexcept {  // что именно должно вернуть значение или узел???
    return pFirst->value;
}

template <typename T>
void TSimpleList<T>::PushFront(const T& val) {
    TNode* tmp;
    if (sz == 0) {
        tmp = new TNode{val, nullptr};
    } else {
        tmp = new TNode{val, pFirst};
    }
    pFirst = tmp;
    sz++;
}

template <typename T>
void TSimpleList<T>::PopFront() noexcept {
    if (sz > 0) {
        TNode* tmp = pFirst->pNext;
        delete pFirst;
        pFirst = tmp;
        sz--;
    }
}

template <typename T>
void TSimpleList<T>::PushAfter(int pos, const T& val) {
    if (pos < 0 || pos > sz - 1) {
        throw out_of_range("Error range for PushAfter");
    } else {
        if (pos == size() - 1) {
        }
        TNode* tmp = this->GetNode(pos);
        TNode* next = tmp->pNext;
        TNode* new_node = new TNode{val, next};
        tmp->pNext = new_node;
        sz++;
    }
}

template <typename T>
void TSimpleList<T>::EraseAfter(int pos) {
    if (pos < 0 || pos > sz - 1 || sz < 2) {
        throw out_of_range("Error range for EraseAfter");
    } else {
        TNode* tmp = this->GetNode(pos);
        TNode* two_next = tmp->pNext->pNext;
        delete tmp->pNext;
        tmp->pNext = two_next;
        sz--;
    }
}

template <typename T>
typename TSimpleList<T>::TNode* TSimpleList<T>::GetNode(int pos) {
    if (pos < 0 || pos > sz - 1) {
        throw out_of_range("Error range for GetNode");
    } else {
        TNode* tmp = pFirst;
        for (int i = 0; i < sz; i++) {
            if (pos == i) {
                return tmp;
            }
            tmp = tmp->pNext;
        }
        throw invalid_argument("Not found value");
    }
}
