#include "pyrochlore.h"
#include "point3d.h"

using namespace std;
using namespace boost;

Pyrochlore::Pyrochlore(int n) : Lattice(n)
{
    typedef color_traits<default_color_type> Color;
    map<point3d,Vertex> points;
    point3d p0, p1;
    int counter = 0;

}

void Pyrochlore::percolate(const double& p, const double& q)
{
}
