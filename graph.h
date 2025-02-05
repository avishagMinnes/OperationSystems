#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <limits>

class Graph {
public:
    Graph() = default;
    
    // הוספת קשת בין שני קודקודים
    void addEdge(int u, int v, double weight);

    // הסרת קשת
    void removeEdge(int u, int v);

    // עדכון משקל קשת
    void updateEdge(int u, int v, double newWeight);

    // קבלת שכנים של קודקוד
    std::vector<std::pair<int, double>> getNeighbors(int u) const;

    // הדפסת הגרף
    void printGraph() const;

    // בדיקת קיום קודקוד
    bool containsVertex(int u) const;

    // בדיקת קיום קשת
    bool containsEdge(int u, int v) const;

    // קבלת כל הקודקודים
    std::vector<int> getVertices() const;

private:
    std::unordered_map<int, std::unordered_map<int, double>> adjList;
};

#endif // GRAPH_H
