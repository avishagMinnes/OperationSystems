#include "mst_factory.h"
#include <queue>
#include <algorithm>

// מבנה עזר ל-Kruskal: Union-Find עם נתיב דחוס
class UnionFind {
private:
    std::vector<int> parent, rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (parent[u] != u) parent[u] = find(parent[u]);
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};

// 🟢 **מימוש Kruskal's Algorithm**
MST computeKruskalMST(const Graph& graph) {
    MST mst(graph);
    std::vector<std::tuple<int, int, double>> edges;

    for (int u : graph.getVertices()) {
        for (auto [v, weight] : graph.getNeighbors(u)) {
            edges.emplace_back(weight, u, v);
        }
    }

    std::sort(edges.begin(), edges.end());  // מיון הקשתות לפי משקל
    UnionFind uf(graph.getVertices().size());

    for (const auto& [weight, u, v] : edges) {
        if (uf.find(u) != uf.find(v)) {
            mst.addEdge(u, v, weight);
            uf.unionSets(u, v);
        }
    }

    return mst;
}

// 🟢 **מימוש Prim's Algorithm**
MST computePrimMST(const Graph& graph) {
    MST mst(graph);
    std::priority_queue<std::tuple<double, int, int>, std::vector<std::tuple<double, int, int>>, std::greater<>> pq;
    std::unordered_set<int> inMST;

    int start = graph.getVertices()[0]; // נבחר קודקוד התחלתי
    inMST.insert(start);

    for (const auto& [v, weight] : graph.getNeighbors(start)) {
        pq.emplace(weight, start, v);
    }

    while (!pq.empty() && inMST.size() < graph.getVertices().size()) {
        auto [weight, u, v] = pq.top();
        pq.pop();

        if (inMST.find(v) == inMST.end()) {
            inMST.insert(v);
            mst.addEdge(u, v, weight);

            for (const auto& [neighbor, w] : graph.getNeighbors(v)) {
                if (inMST.find(neighbor) == inMST.end()) {
                    pq.emplace(w, v, neighbor);
                }
            }
        }
    }

    return mst;
}

// 🏭 **Factory ליצירת האלגוריתם המתאים**
MST MSTFactory::computeMST(const Graph& graph, MSTAlgorithm algorithm) {
    if (algorithm == KRUSKAL) {
        return computeKruskalMST(graph);
    } else {
        return computePrimMST(graph);
    }
}
