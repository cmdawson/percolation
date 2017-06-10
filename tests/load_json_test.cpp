#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "graph.h"
#include "unitcell.h"

using namespace std;
 
 
BOOST_AUTO_TEST_CASE(universeInOrder)
{
	UnitCell C;
	ifstream inf;
	inf.open("data/honeycomb.json");

	inf >> C;

    BOOST_CHECK(4 == 4);
}
