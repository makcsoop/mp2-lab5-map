#include <vector>

#include "log.h"

using namespace std;

template <typename T>
class DHeap {
    vector<T> data;
    int d;

   public:
    DHeap() : d(1) {};

    DHeap(vector<T> data, int d) {
        this->d = d;
        heapify(data, d);
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
                    T tmp = data[index_parent];
                    data[index_parent] = data[current_index];
                    data[current_index] = tmp;
                    current_index = index_parent;
                    index_parent = (current_index - 1) / d;
                }
            }
        }
        logger("SUCCESS HEAP GENERAT", 1);
    }

    void Insert(T key) { data.push_back(key); }

    int find(T key) {
        for (int i = 0; i < static_cast<int>(data.size()); i++) {
            if (data[i] == key) {
                return i;
            }
        }
        throw invalid_argument("non key");
    }

    int min_child_index(int index) {
        int res = index * d + 1;
        for (int i = index * d + 2; i < min(static_cast<int>(data.size()), index * d + d + 1); i++) {
            if (data[res] > data[i]) {
                res = i;
            }
        }
        return res;
    }

    void decreaseKey(T key, T new_key) {
        int index = find(key);
        int index_parent = (index - 1) / d;
        data[index] = new_key;
        if (data[index_parent] > new_key) {
            while (index != 0 && data[index_parent] > new_key) {
                swap(data[index_parent], data[index]);
                index = index_parent;
                index_parent = (index - 1) / d;
            }

        } else if (data[min_child_index(index)] < new_key) {
            // cout << index << " " << data[min_child_index(index)] << endl;
            int index_min = min_child_index(index);
            while (data[index_min] < new_key) {
                swap(data[index_min], data[index]);
                index = index_min;
                index_min = min_child_index(index);
            }
        }
    }

    T extractMin() {
        T res = data[0];
        if (data.size() > 1) {
            data[0] = data[data.size() - 1];
            data.pop_back();
            T current = data[0];
            int index = 0;
            int min_index = (index * d + 1);
            T min_key = data[index * d + 1];
            for (int i = (index * d + 2); i < (min(index * d + d + 1, (int)data.size())); i++) {
                if (data[i] < min_key) {
                    min_index = i;
                    min_key = data[min_index];
                }
            }
            data[min_index] = current;
            data[index] = min_key;
            while (current < (int)data.size() && current > min_key) {
                index = min_index;
                min_index = index * d + 1;
                if (min_index > (int)data.size() - 1) {
                    break;
                }
                T min_key = data[index * d + 1];
                for (int i = (index * d + 2); i < (min(index * d + d + 1, (int)data.size())); i++) {
                    if (data[i] < min_key) {
                        min_index = i;
                        min_key = data[min_index];
                    }
                }
                data[min_index] = current;
                data[index] = min_key;
            }
            logger("DELETE SUSSCES", 1);
        }

        return res;
    }

    T getMin() { return data[0]; }

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