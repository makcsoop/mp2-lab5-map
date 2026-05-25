#include <vector>

#include "log.h"

using namespace std;

template <typename T>
class DHeap {
    vector<T> data;
    int d;

   public:
    DHeap() : d(2) {};

    DHeap(vector<T> data, int d) {
        this->d = d;
        heapify(data, d);
    }

    DHeap(DHeap<T>& tmp) {
        d = tmp.d;
        data = tmp.data;
    }

    DHeap(DHeap<T>&& tmp) : data(std::move(tmp.data)), d(tmp.d) { tmp.d = 0; }

    DHeap<T>& operator=(const DHeap<T>& s) {
        if (this != &s) {
            this->data = s.data;
            this->d = s.d;
        }
        return *this;
    }

    void heapify(vector<T> new_data, int d) {
        if (new_data.size() <= 0) {
            throw invalid_argument("size 0");
        }
        data.clear();
        for (int i = 0; i < static_cast<int>(new_data.size()); i++) {
            data.push_back(new_data[i]);
            if (i != 0) {
                int current_index = i;
                int index_parent = (current_index - 1) / d;
                while (current_index > 0 && data[current_index] < data[index_parent]) {
                    swap(data[current_index], data[index_parent]);
                    current_index = index_parent;
                    index_parent = (current_index - 1) / d;
                }
            }
        }
    }

    void Insert(T key) {
        data.push_back(key);
        int index = data.size() - 1;

        while (index > 0) {
            int parent = (index - 1) / d;
            if (data[index] < data[parent]) {
                swap(data[index], data[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    int find(T key) {
        for (int i = 0; i < static_cast<int>(data.size()); i++) {
            if (data[i] == key) {
                return i;
            }
        }
        throw invalid_argument("non key");
    }

    int min_child_index(int index) {
        int first_child = index * d + 1;
        if (first_child >= static_cast<int>(data.size())) {
            return -1;
        }

        int res = first_child;
        for (int i = first_child + 1; i < min(static_cast<int>(data.size()), index * d + d + 1); i++) {
            if (data[res] > data[i]) {
                res = i;
            }
        }
        return res;
    }

    int size() { return static_cast<int>(data.size()); }

    bool IsEmpty() { return static_cast<int>(data.size()) == 0; }

    T getNode(int index) { return data[index]; }

    void decreaseKey(T key, T new_key) {
        if (new_key > key) {
            throw invalid_argument("new_key must be less than old key");
        }

        int index = find(key);
        data[index] = new_key;

        while (index > 0) {
            int parent = (index - 1) / d;
            if (data[parent] > data[index]) {
                swap(data[parent], data[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    T extractMin() {
        if (static_cast<int>(data.size()) < 1) {
            throw invalid_argument("Size 0");
        }
        T res = data[0];
        data[0] = data[0] = data[data.size() - 1];
        int current_index = 0;
        int child_index = min_child_index(current_index);
        data.pop_back();
        while (data.size() > 0 && child_index != -1 && data[current_index] > data[child_index]) {
            swap(data[current_index], data[child_index]);
            current_index = child_index;
            child_index = min_child_index(current_index);
        }
        return res;
    }

    T getMin() {
        if (static_cast<int>(data.size()) < 1) {
            throw invalid_argument("Size 0");
        }
        return data[0];
    }

    void Print() {
        cout << "[";
        for (int i = 0; i < static_cast<int>(data.size()); i++) {
            if (i == static_cast<int>(data.size()) - 1) {
                cout << data[i];
            } else {
                cout << data[i] << ", ";
            }
        }
        cout << "]\n";
    }
};