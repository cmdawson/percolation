#ifndef _INC_PERC_UNITCELL_H
#define _INC_PERC_UNITCELL_H
#include "graph.h"
#include <stdexcept>

class UnitCell : public Graph
{
private:
	std::vector<coordinate> _axes;

	void determine_boundaries(void);

public:
	std::vector<coordinate>& axes() { return _axes; }
	const std::vector<coordinate>& axes() const { return _axes; }
	void axes(const std::vector<coordinate>& a) { _axes = a; }

	UnitCell() : Graph() {}
	~UnitCell() {}

	// TODO: Copy constructor for UnitCell

    friend std::ostream& operator<<(std::ostream &output, const UnitCell &C);
	friend std::istream& operator>>(std::istream &input, UnitCell &C);
};
#endif
