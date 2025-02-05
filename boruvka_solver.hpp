#ifndef BORUVKA_SOLVER_HPP
#define BORUVKA_SOLVER_HPP

#include "mst_solver.hpp"
#include <vector>
#include <limits>
#include <iostream>
#include <tuple>


class BoruvkaSolver : public MSTSolver {
public:
    int computeMST(int n, const std::vector<std::list<std::pair<int, int>>>& graph,
                   std::vector<std::list<std::pair<int, int>>>& mstGraph) override {
        
        std::vector<int> parent(n + 1), rank(n + 1, 0);
        for (int i = 1; i <= n; ++i) parent[i] = i;

        auto find = [&](int x) {
            while (x != parent[x]) x = parent[x];
            return x;
        };

        auto unionSets = [&](int x, int y) {
            int rootX = find(x), rootY = find(y);
            if (rootX != rootY) {
                if (rank[rootX] > rank[rootY]) parent[rootY] = rootX;
                else if (rank[rootX] < rank[rootY]) parent[rootX] = rootY;
                else {
                    parent[rootY] = rootX;
                    rank[rootX]++;
                }
                return true;
            }
            return false;
        };

        int mstWeight = 0;
        int components = n;

        while (components > 1) {
            std::vector<std::tuple<int, int, int>> minEdge(n + 1, {std::numeric_limits<int>::max(), -1, -1});

            for (int u = 1; u <= n; ++u) {
                for (const auto& [v, weight] : graph[u]) {
                    int setU = find(u), setV = find(v);
                    if (setU != setV) {
                        if (std::get<0>(minEdge[setU]) > weight) {
                            minEdge[setU] = {weight, u, v};
                        }
                        if (std::get<0>(minEdge[setV]) > weight) {
                            minEdge[setV] = {weight, u, v};
                        }
                    }
                }
            }

            for (int i = 1; i <= n; ++i) {
                if (std::get<1>(minEdge[i]) != -1) {
                    int u = std::get<1>(minEdge[i]), v = std::get<2>(minEdge[i]), weight = std::get<0>(minEdge[i]);
                    if (unionSets(u, v)) {
                        mstGraph[u].push_back({v, weight});
                        mstWeight += weight;
                        components--;
                    }
                }
            }
        }

        return mstWeight;
    }
};

#endif // BORUVKA_SOLVER_HPP
