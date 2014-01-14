#ifndef _INC_PERC_XYZCOORD_T
#define _INC_PERC_XYZCOORD_T

#include <cmath>
#include "lattice.h"

class point3d
{
public:
    double x,y,z,epsilon;
    
    point3d(double eps=1.0E-6) : epsilon(eps) {}
    point3d(double _x, double _y, double _z, double eps=1.0E-9) 
	: x(_x), y(_y), z(_z), epsilon(eps) {}

    bool operator<(const point3d& rhs) const
    {
	if (abs(rhs.x-x) < epsilon)
	{
	    if (abs(rhs.y-y) < epsilon)
	    {
		return (rhs.z-z) > epsilon;
	    }
	    return (rhs.y-y) > epsilon;
	}
	return (rhs.x-x) > epsilon;
    }

    point3d& operator+=(const point3d& rhs)
    {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
    }

    point3d& operator-=(const point3d& rhs)
    {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
    }

    const point3d operator+(const point3d &rhs) const
    {
	point3d result = *this;
	result += rhs;
	return result;
    }

    const point3d operator-(const point3d &rhs) const
    {
	point3d result = *this;
	result -= rhs;
	return result;
    }


    point3d& operator*=(const double& alpha)
    {
	x *= alpha;
	y *= alpha;
	z *= alpha;
	return *this;
    }

    static Lattice::Vertex add_as_vertex(const point3d& p, int ii, Lattice::AdjacencyList& adj)
    {
	typedef boost::color_traits<boost::default_color_type> Color;
	Lattice::Vertex v = boost::add_vertex(adj);
	adj[v].x = p.x;
	adj[v].y = p.y;
	adj[v].z = p.z;
	adj[v].index = ii;
	adj[v].color = Color::white();

	return v;
    }

};

inline point3d operator*(const double& alpha, const point3d& rhs)
{
    point3d ret = rhs;
    return ret *= alpha;
}

#endif
