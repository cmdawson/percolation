include "unitcell.h"

class Lattice : public Graph
{
private:
	std::vector<vertex_descriptor> _index;
	std::map<coordinate&

	UnitCell& _unitCell;

	void add_cells(unsigned dim, vector<unsigned>& icoords)
	{
		for (icoords[dim]=0; icoords[dim]<_extants[dim]; icoords[dim]++)
		{
			add_cell(icoords);

			if (dim > 0)
			{
				add_cells(dim-1, icoords);
			}
		}
	}

	void add_cell(const vector<unsigned>& icoords)
	{
		// Translate the unit-cell by the icoords
		// 

	}



public:
	Lattice(const UnitCell& U, unsigned n)
	{
		size_t dim = U.axes().size();

		if (dim < 1)
		{
			throw std::runtime_error("No axes found in unit cell");
		}

		vector<int> ii(dim, 0);




		// Iterate in each dimension from 0 to n-1
		// probably requires recursion

	}



	~Lattice() {}


};
