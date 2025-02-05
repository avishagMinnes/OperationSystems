#include "mst.h"
#include <iostream>
#include <queue>
#include <unordered_set>

MST::MST(const Graph& graph) : graph(graph) {}

void MST::addEdge(int u, int v, double weight) {
    mstEdges.push_back({u, v, weight});
}

double MST::getTotalWeight() const {
    double totalWeight = 0.0;
    for (const auto& [u, v, weight] : mstEdges) {
        totalWeight += weight;
    }
    return totalWeight;
}

double MST::getLongestDistance() const {
    double longest = 0.0;
    for (const auto& [u, v, weight] : mstEdges) {
        if (weight > longest) {
            longest = weight;
        }
    }
    return longest;
}

double MST::getAverageDistance() const {
    if (mstEdges.empty()) return 0.0;
    
    double total = 0.0;
    int count = 0;
    
    for (const auto& [u, v, weight] : mstEdges) {
        total += weight;
        count++;
    }
    
    return count ? (total / count) : 0.0;
}

double MST::getShortestDistance() const {
    double shortest = std::numeric_limits<double>::max();
    
    for (const auto& [u, v, weight] : mstEdges) {
        if (weight < shortest) {
            shortest = weight;
        }
    }
    
    return (shortest == std::numeric_limits<double>::max()) ? 0.0 : shortest;
}

void MST::printMST() const {
    std::cout << "MST Edges:\n";
    for (const auto& [u, v, weight] : mstEdges) {
        std::cout << u << " - " << v << " (weight: " << weight << ")\n";
    }
}
