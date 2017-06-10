#include "unitcell.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_tree;


istream& operator>>(istream &input, UnitCell &C)
{
	ptree root;
	read_json(input, root);
	C.read_from_ptree(root);

	optional<ptree&> axes = root.get_child_optional("axes");
	if (axes)
	{
		for (ptree::value_type &axis : *axes)
		{
			vector<double> v;
			for (ptree::value_type &elmt : axis.second)
			{
				v.push_back(elmt.second.get_value<double>());
			}
			C.axes().push_back(Graph::coordinate(v));
		}
	}
	return input;
}

ostream &operator<<(ostream &output, const UnitCell& C)
{
	property_tree::ptree root;

	if (C.axes().size() > 0)
	{
		ptree matrix;
		for (const auto& axis : C.axes())
		{
			ptree row;
			for (const auto& elmt : axis)
			{
				ptree cell;
				cell.put_value(elmt);
        		row.push_back(make_pair("", cell));
			}
			matrix.push_back(make_pair("",row));
		}
		root.add_child("axes", matrix);
	}

	C.write_to_ptree(root);
	write_json(output, root, false);

	return output;
}
