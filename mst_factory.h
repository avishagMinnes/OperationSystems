#ifndef MST_FACTORY_H
#define MST_FACTORY_H

#include "graph.h"
#include "mst.h"

enum MSTAlgorithm {
    KRUSKAL,
    PRIM
};

class MSTFactory {
public:
    static MST computeMST(const Graph& graph, MSTAlgorithm algorithm);
};

#endif // MST_FACTORY_H
