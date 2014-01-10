#ifndef _INC_PERC_HONEYCOMB_H
#define _INC_PERC_HONEYCOMB_H

#include "lattice.h"

class Honeycomb : public Lattice
{
public:
    Honeycomb(int n);
    ~Honeycomb() {}

    void percolate(const double& p, const double& q);
};

#endif


