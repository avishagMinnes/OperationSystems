#include "graph.hpp"

Graph::Graph(int n) : n(n) {
    graph.resize(n + 1);  // 1-based index
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 1 || u > n || v < 1 || v > n) {
        std::cerr << "Error: Invalid edge (" << u << ", " << v << ").\n";
        return;
    }
    graph[u].push_back({v, weight});
}

void Graph::removeEdge(int u, int v) {
    if (u < 1 || u > n || v < 1 || v > n) {
        std::cerr << "Error: Invalid edge removal (" << u << ", " << v << ").\n";
        return;
    }
    
    // Remove the edge (u, v) regardless of weight
    graph[u].remove_if([v](const std::pair<int, int>& edge) {
        return edge.first == v;
    });
}

void Graph::printGraph() const {
    for (int i = 1; i <= n; ++i) {
        std::cout << i << ": ";
        for (const auto& neighbor : graph[i]) {
            std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        std::cout << "\n";
    }
}







