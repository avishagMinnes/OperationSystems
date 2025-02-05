#include "graph.hpp"

Graph::Graph(int n) : n(n) {
    graph.resize(n + 1);
    mstGraph.resize(n + 1);
}

void Graph::addEdge(int u, int v, int weight) {
    if (u < 1 || u > n || v < 1 || v > n) {
        std::cerr << "Error: Invalid edge (" << u << " -> " << v << ").\n";
        return;
    }
    graph[u].push_back({v, weight}); // Directed edge from u -> v
}

void Graph::removeEdge(int u, int v) {
    if (u < 1 || u > n || v < 1 || v > n) {
        std::cerr << "Error: Invalid edge removal (" << u << " -> " << v << ").\n";
        return;
    }
    graph[u].remove_if([v](const std::pair<int, int>& edge) {
        return edge.first == v;
    });
}

void Graph::printGraph() const {
    for (int i = 1; i <= n; ++i) {
        std::cout << i << " -> ";
        for (const auto& neighbor : graph[i]) {
            std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        std::cout << "\n";
    }
}

// Compute MST for a directed graph using the selected algorithm
void Graph::computeMST(MSTAlgorithm algo, int root) {
    auto solver = MSTFactory::createSolver(algo);
    mstGraph.assign(n + 1, {}); // Reset MST graph
    mstWeight = solver->computeMST(n, graph, mstGraph, root);
    computeDistances();
}

// Compute shortest paths and MST-related metrics
void Graph::computeDistances() {
    std::vector<std::vector<int>> dist(n + 1, std::vector<int>(n + 1, std::numeric_limits<int>::max()));

    for (int i = 1; i <= n; ++i) {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        dist[i][i] = 0;
        pq.push({0, i});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[i][u]) continue;

            for (const auto& [v, w] : mstGraph[u]) {
                if (dist[i][v] > dist[i][u] + w) {
                    dist[i][v] = dist[i][u] + w;
                    pq.push({dist[i][v], v});
                }
            }
        }
    }

    longestDistance = 0;
    shortestDistance = std::numeric_limits<int>::max();
    long long totalDistance = 0;
    int count = 0;

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            if (dist[i][j] < std::numeric_limits<int>::max()) {
                longestDistance = std::max(longestDistance, dist[i][j]);
                shortestDistance = std::min(shortestDistance, dist[i][j]);
                totalDistance += dist[i][j];
                count++;
            }
        }
    }

    averageDistance = count > 0 ? (double)totalDistance / count : 0;
}

int Graph::getMSTTotalWeight() const {
    return mstWeight;
}

int Graph::getMSTLongestDistance() const {
    return longestDistance;
}

double Graph::getMSTAverageDistance() const {
    return averageDistance;
}

int Graph::getMSTShortestDistance() const {
    return shortestDistance;
}








