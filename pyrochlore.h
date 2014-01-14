#ifndef _INC_PERC_PYROCHLORE_H
#define _INC_PERC_PYROCHLORE_H

#include "diamond.h"

class Pyrochlore : public Diamond
{
private:
    AdjacencyList diamond_adjacency;
public:
    Pyrochlore(int n);
    ~Pyrochlore() {}

    void percolate(const double& p, const double& q);
};

#endif


