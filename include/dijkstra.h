#include <iostream>
#include <map>
#include <vector>

#include "D-heap.h"
#include "log.h"

using namespace std;

class Dijkstra {
    int start;
    int end;
    map<int, vector<pair<int, int>>>& graph;

   public:
    Dijkstra(map<int, vector<pair<int, int>>>& graph, int _start, int _end)
        : start(_start), end(_end), graph(graph) {}

    map<int, int> EtalonCalculate() {
        map<int, int> distances;
        for (const auto& node : graph) {
            distances[node.first] = INT_MAX;
        }
        distances[start] = 0;

        map<int, bool> visited;

        for (size_t i = 0; i < graph.size(); ++i) {
            int min_distance = INT_MAX;
            int min_node = -1;

            for (const auto& node : graph) {
                if (!visited[node.first] && distances[node.first] < min_distance) {
                    min_distance = distances[node.first];
                    min_node = node.first;
                }
            }

            if (min_node == -1) break;

            visited[min_node] = true;

            for (const auto& neighbor : graph[min_node]) {
                int new_distance = distances[min_node] + neighbor.second;
                if (new_distance < distances[neighbor.first]) {
                    distances[neighbor.first] = new_distance;
                }
            }
        }

        return distances;
    }

    vector<int> Calculate() {
        int n = graph.size();
        vector<int> dist(n, INT_MAX);
        vector<bool> isposh(n, 0);

        dist[start] = 0;
        DHeap<pair<int, int>> priority;  // вес - номер_вершины  first-вес   second-номер
        // graph first-номер   second-вес
        priority.Insert({0, start});
        while (!priority.IsEmpty()) {
            auto node = priority.extractMin();
            // int current_dist = node.first;
            int current_node = node.second;

            isposh[current_node] = 1;
            vector<pair<int, int>>& heigbourd = graph[current_node];
            for (auto x : heigbourd) {
                if (isposh[x.first]) continue;

                if (dist[current_node] + x.second < dist[x.first]) {
                    dist[x.first] = dist[current_node] + x.second;
                    // if ()
                    priority.Insert(pair<int, int>{dist[x.first], x.first});
                }
            }
            logger("len q: " + to_string(priority.size()), 1);
        }

        for (int i = 0; i < static_cast<int>(dist.size()); i++) {
            cout << dist[i] << " ";
        }
        cout << "\n";

        return dist;
    }
};