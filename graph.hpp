#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>
#include <limits>
#include "mst_factory.hpp"

class Graph {
public:
    Graph(int n);
    void addEdge(int u, int v, int weight);
    void removeEdge(int u, int v);
    void printGraph() const;

    // Compute MST for directed graph (without root)
    void computeMST(MSTAlgorithm algo);

    // MST Metrics
    int getMSTTotalWeight() const;
    int getMSTLongestDistance() const;
    double getMSTAverageDistance() const;
    int getMSTShortestDistance() const;

private:
    int n;
    std::vector<std::list<std::pair<int, int>>> graph;   // Adjacency list for directed graph
    std::vector<std::list<std::pair<int, int>>> mstGraph; // Directed MST

    int mstWeight = 0;
    int longestDistance = 0;
    double averageDistance = 0;
    int shortestDistance = std::numeric_limits<int>::max();

    void computeDistances();
};

#endif // GRAPH_H


