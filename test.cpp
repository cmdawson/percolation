#include "pyrochlore.h"
//#include "diamond.h"
//#include "honeycomb.h"
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

    unsigned int N = atoi(argv[1]);
    unsigned int SAMPLE = atoi(argv[2]);
    double p = strtod(argv[3],0);
    double Q = strtod(argv[4],0);
    cout << "# n=" << N << endl;
   
    //Diamond DM(N);	
    Pyrochlore PC(N);

    //Honeycomb H(N);	// virgin honeycomb

    unsigned ncrossed = 0;
    for (unsigned j=0;j<SAMPLE;j++)
    {
	Pyrochlore P = PC;
	// Diamond P = DM; 
	//Honeycomb P = H;

	//P.percolate(p, Q); //Jan 21:still to do

       ncrossed += P.is_crossable();
    }

    cout << p << " " << ncrossed << " " << SAMPLE << " " 
	<< 1.0*ncrossed/SAMPLE << endl;
}


void print_run_error(void)
{
	cerr << "Usage: main.exe N Sample p q" << endl << endl << "where" << endl 
	    << "N means and NxN pyrochlore lattice" << endl
	    << "Sample is the number of trials" << endl
	    << "p is the probability of vertex being there"<<endl
	    << "q is the probability of the edge being there" << endl;
	cerr << "\nThe algorithm then tries a range of vertex occupation probabilities"
	    "\np to establish the threshold for this q.\n";
}
