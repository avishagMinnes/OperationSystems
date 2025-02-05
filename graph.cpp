#include "graph.hpp"
#include <stack>
#include <algorithm>

Graph::Graph(int n, int m) : n(n), m(m) {
    graph.resize(n + 1);
}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
}

void Graph::removeEdge(int u, int v) {
    graph[u].remove(v);
}






