#ifndef MST_H
#define MST_H

#include "graph.h"
#include <vector>
#include <limits>

class MST {
public:
    MST(const Graph& graph); 

    void addEdge(int u, int v, double weight);
    double getTotalWeight() const;
    double getLongestDistance() const;
    double getAverageDistance() const;
    double getShortestDistance() const;

    void printMST() const;

private:
    std::vector<std::tuple<int, int, double>> mstEdges;
    const Graph& graph;
};

#endif // MST_H
