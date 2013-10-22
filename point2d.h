#ifndef _INC_PERC_XYCOORD_T
#define _INC_PERC_XYCOORD_T

#include <cmath>

class point2d
{
public:
	double x,y,epsilon;
	
	point2d(double eps=1.0E-6) : epsilon(eps) {}
	
	bool operator<(const point2d& rhs) const
	{
		if (abs(rhs.x-x) < epsilon)
			return (rhs.y-y) > epsilon;
		return (rhs.x-x) > epsilon;
	}
};

#endif