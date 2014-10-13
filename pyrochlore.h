#ifndef _INC_PERC_PYROCHLORE_H
#define _INC_PERC_PYROCHLORE_H

#include "diamond.h"

class Pyrochlore : public Diamond
{
private:
    AdjacencyList diamond_adjacency;
public:
    int size;
    Pyrochlore(int n, int ny, int nz, double p, bool Firstrun);
    ~Pyrochlore() {}

    void percolate(const double& p, const double& q);
};

#endif


