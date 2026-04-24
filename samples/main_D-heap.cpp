#include <vector>

#include "../include/D-heap.h"

int main() {  // изменение ключа + алгоритм дейсктры
    vector<int> data = {33, 53, 645645, 4, 4242, 232, 21, 41241, 421, 1, 42145, 5, 4};
    DHeap<int> heap(data, 3);
    heap.Print();
    // cout << "start extract \n";
    cout << heap.extractMin() << endl;
    heap.Insert(1000000);
    heap.Print();
}