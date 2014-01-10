#include "lattice.h"

using namespace std;
using namespace boost;


random::uniform_01<random::mt19937,double> Lattice::random01(random::mt19937(42));

ostream &operator<<(ostream &output, const Lattice &H)
{
    output << "boo";
}
