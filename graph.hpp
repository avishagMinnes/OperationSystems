#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <iostream>

class Graph {
public:
    Graph(int n);
    void addEdge(int u, int v, int weight);
    void removeEdge(int u, int v);
    void printGraph() const;  // Utility function to print the graph

private:
    int n;
    std::vector<std::list<std::pair<int, int>>> graph; // Adjacency list storing (neighbor, weight)
};

#endif // GRAPH_H

