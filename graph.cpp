#include "graph.hpp"
#include <stack>
#include <algorithm>

Graph::Graph(int n, int m) : n(n), m(m) {
    graph.resize(n + 1);
    reverseGraph.resize(n + 1);
}

void Graph::addEdge(int u, int v) {
    graph[u].push_back(v);
    reverseGraph[v].push_back(u);
}

void Graph::removeEdge(int u, int v) {
    graph[u].remove(v);
    reverseGraph[v].remove(u);
}






