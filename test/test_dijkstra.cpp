#include <gtest.h>

#include <map>

#include "dijkstra.h"

// g++ test/*.cpp -I./include  -I./gtest  gtest/*.cc -fsanitize=address  -fsanitize=leak

TEST(DijkstraTest, DijkstraTestEtalon) {
    map<int, vector<pair<int, int>>> graph = {
        {0, {{1, 4}, {2, 2}}}, {1, {{2, 1}, {3, 5}}}, {2, {{3, 8}}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 3);
    cout << "Shortest path from 0 to 3: " << dijkstra.Calculate() << endl;
    EXPECT_EQ(dijkstra.Calculate(), dijkstra.EtalonCalculate());
}

TEST(DijkstraTest, SimpleGraph) {
    map<int, vector<pair<int, int>>> graph = {
        {0, {{1, 4}, {2, 2}}}, {1, {{2, 1}, {3, 5}}}, {2, {{3, 8}}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 3);
    vector<int> result = dijkstra.Calculate();
    int expected = dijkstra.EtalonCalculate();

    cout << "Shortest path from 0 to 3: " << result << endl;
    EXPECT_EQ(result, expected);
}

TEST(DijkstraTest, SimpleGraphCorrect) {
    map<int, vector<pair<int, int>>> graph = {
        {0, {{1, 4}, {2, 2}}}, {1, {{2, 1}, {3, 5}}}, {2, {{3, 8}}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 3);
    int result = dijkstra.Calculate();
    int expected = dijkstra.EtalonCalculate();

    EXPECT_EQ(result, expected);
    EXPECT_EQ(result, 9);
}

TEST(DijkstraTest, DirectPath) {
    map<int, vector<pair<int, int>>> graph = {{0, {{1, 5}}}, {1, {{2, 3}}}, {2, {{3, 2}}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 3);
    int result = dijkstra.Calculate();
    int expected = dijkstra.EtalonCalculate();

    EXPECT_EQ(result, expected);
    EXPECT_EQ(result, 10);
}

TEST(DijkstraTest, StartEqualsEnd) {
    map<int, vector<pair<int, int>>> graph = {{0, {{1, 5}}}, {1, {{2, 3}}}, {2, {}}, {3, {}}};

    Dijkstra dijkstra(graph, 0, 0);
    int result = dijkstra.Calculate();
    int expected = dijkstra.EtalonCalculate();

    EXPECT_EQ(result, expected);
    EXPECT_EQ(result, 0);
}