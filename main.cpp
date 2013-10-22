#include "honeycomb.h"
#include <iostream>

using namespace std;
using namespace boost;

void print_run_error(void);

int main(int argc, char* argv[])
{	
	if (argc != 5)
	{
		print_run_error();
		exit(0);
	}

	unsigned int NHEX = atoi(argv[1]);
	unsigned int SAMPLE = atoi(argv[2]);
	double p0 = strtod(argv[3],0);
	double Q = strtod(argv[4],0);
	cout << "# nhex=" << NHEX << endl;
	
	Honeycomb H(NHEX);	// virgin honeycomb

	// When q=1.0 the vertex percolation threshold occurs at 0.69704, while
	// when p=1.0
    

	unsigned ncrossed = 0;
	for (unsigned j=0;j<SAMPLE;j++)
	{
	    Honeycomb P = H;
	    P.percolate(p, Q);
	    ncrossed += P.is_crossable();
	    }
	    cout << p << " " << ncrossed << " " << SAMPLE << endl;
	}
}


void print_run_error(void)
{
	cerr << "Usage: main.exe H N q" << endl << endl << "where" << endl 
			<< "H means and HxH honeycomb lattice" << endl
			<< "N is the number of trials" << endl
			<< "q is the probability of the edge being there" << endl;
	cerr << "\nThe algorithm then tries a range of vertex occupation probabilities"
	    "\np to establish the threshold for this q. It tries to make an educated"
	    "\nguess for the range of p it needs to sample\n";
}
