#ifndef MST_FACTORY_HPP
#define MST_FACTORY_HPP

#include "mst_solver.hpp"
#include "prim_solver.hpp"
#include "boruvka_solver.hpp"
#include <memory>

enum class MSTAlgorithm {
    PRIM,
    BORUVKA
};

class MSTFactory {
public:
    static std::unique_ptr<MSTSolver> createSolver(MSTAlgorithm algo) {
        if (algo == MSTAlgorithm::PRIM) {
            return std::make_unique<PrimSolver>();
        } else if (algo == MSTAlgorithm::BORUVKA) {
            return std::make_unique<BoruvkaSolver>();
        }
        return nullptr;
    }
};

#endif // MST_FACTORY_HPP

