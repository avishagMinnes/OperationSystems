#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <unordered_map>
#include <functional>

class Graph {
public:
    Graph(int n, int m);
    void addEdge(int u, int v);
    void removeEdge(int u, int v);

private:
    int n, m;
    std::vector<std::list<int>> graph;
};

#endif // GRAPH_H
