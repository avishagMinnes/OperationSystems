#ifndef MST_SOLVER_HPP
#define MST_SOLVER_HPP

#include <vector>
#include <list>

class MSTSolver {
public:
    virtual ~MSTSolver() = default;
    virtual int computeMST(int n, const std::vector<std::list<std::pair<int, int>>>& graph,
                           std::vector<std::list<std::pair<int, int>>>& mstGraph) = 0;
};

#endif // MST_SOLVER_HPP


