#ifndef _INC_PERC_XYZCOORD_T
#define _INC_PERC_XYZCOORD_T

#include <cmath>

class point3d
{
public:
    double x,y,z,epsilon;
    
    point3d(double eps=1.0E-9) : epsilon(eps) {}

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
};

#endif
