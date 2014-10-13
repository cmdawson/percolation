#ifndef _INC_PERC_DIAMOND_H
#define _INC_PERC_DIAMOND_H

#include "lattice.h"

class Diamond : public Lattice
{
public:
    Diamond(int n, int ny, int nz);
    ~Diamond();

    void percolate(const double& p, const double& q);
};

#endif


