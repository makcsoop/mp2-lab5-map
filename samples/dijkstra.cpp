#include "dijkstra.h"

#include <iostream>
#include <map>
#include <utility>
#include <vector>

using namespace std;

int main() {
    map<int, vector<pair<int, int>>> graph = {
        {0, {{1, 4}, {2, 2}}}, {1, {{2, 1}, {3, 5}}}, {2, {{3, 8}}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 3);
    auto res = dijkstra.Calculate();
    for (int i = 0; i < static_cast<int>(res.size()); i++) {
        cout << "Node " << i << " - " << res[i] << endl;
    }
    // cout << "Shortest path from 0 to 3: " << dijkstra.Calculate() << endl;
};