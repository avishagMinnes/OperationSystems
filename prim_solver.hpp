#ifndef PRIM_SOLVER_HPP
#define PRIM_SOLVER_HPP

#include "mst_solver.hpp"
#include <queue>
#include <limits>

class PrimSolver : public MSTSolver {
public:
    int computeMST(int n, const std::vector<std::list<std::pair<int, int>>>& graph,
                   std::vector<std::list<std::pair<int, int>>>& mstGraph) override {
        std::vector<bool> inMST(n + 1, false);
        std::priority_queue<std::pair<int, std::pair<int, int>>,
                            std::vector<std::pair<int, std::pair<int, int>>>,
                            std::greater<>> pq;

        inMST[1] = true;
        for (const auto& edge : graph[1]) {
            pq.push({edge.second, {1, edge.first}});
        }

        int mstWeight = 0, edgesAdded = 0;

        while (!pq.empty() && edgesAdded < n - 1) {
            auto [weight, edge] = pq.top();
            pq.pop();
            int u = edge.first, v = edge.second;

            if (inMST[v]) continue;
            
            mstWeight += weight;
            inMST[v] = true;
            mstGraph[u].push_back({v, weight});
            mstGraph[v].push_back({u, weight});
            edgesAdded++;

            for (const auto& nextEdge : graph[v]) {
                if (!inMST[nextEdge.first]) {
                    pq.push({nextEdge.second, {v, nextEdge.first}});
                }
            }
        }

        return mstWeight;
    }
};

#endif // PRIM_SOLVER_HPP
