#include "graph.h"

void Graph::addEdge(int u, int v, double weight) {
    adjList[u][v] = weight;
}

void Graph::removeEdge(int u, int v) {
    if (adjList.find(u) != adjList.end()) {
        adjList[u].erase(v);
        if (adjList[u].empty()) {
            adjList.erase(u);
        }
    }
}

void Graph::updateEdge(int u, int v, double newWeight) {
    if (containsEdge(u, v)) {
        adjList[u][v] = newWeight;
    }
}

std::vector<std::pair<int, double>> Graph::getNeighbors(int u) const {
    std::vector<std::pair<int, double>> neighbors;
    if (adjList.find(u) != adjList.end()) {
        for (const auto& pair : adjList.at(u)) {
            neighbors.emplace_back(pair.first, pair.second);
        }
    }
    return neighbors;
}

void Graph::printGraph() const {
    for (const auto& node : adjList) {
        std::cout << "Vertex " << node.first << " -> ";
        for (const auto& edge : node.second) {
            std::cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        std::cout << std::endl;
    }
}

bool Graph::containsVertex(int u) const {
    return adjList.find(u) != adjList.end();
}

bool Graph::containsEdge(int u, int v) const {
    return adjList.find(u) != adjList.end() && adjList.at(u).find(v) != adjList.at(u).end();
}

std::vector<int> Graph::getVertices() const {
    std::vector<int> vertices;
    for (const auto& node : adjList) {
        vertices.push_back(node.first);
    }
    return vertices;
}
