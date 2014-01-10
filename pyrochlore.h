#ifndef _INC_PERC_PYROCHLORE_H
#define _INC_PERC_PYROCHLORE_H

#include "lattice.h"

class Pyrochlore : public Lattice
{
public:
    Pyrochlore(int n);
    ~Pyrochlore() {}

    void percolate(const double& p, const double& q);
};

#endif


