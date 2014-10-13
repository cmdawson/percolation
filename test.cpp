#include "pyrochlore.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include<math.h>

using namespace std;
using namespace boost;

void print_run_error(void);

int main(int argc, char* argv[])
{	
//no reason for any input (following 5 lines) in this version since the probabilities and lattice sizes don't need to be asked for.
//	if (argc != 7)
//	{
//		print_run_error();
//		exit(0);
//	}

	cout<<"This code scans fusion probabilities for a pyrochlore lattice of size 2x2x to 16x16x16 to show that 75% fusion probability is above the percolation threshold given the lattice generation scheme presented in the accompanying paper. Probabilities will be scanned from 0.69 to 0.71 in increments of 0.001, and each lattice size will be sampled 1000 times. Also, input Q=1 to match the scheme of the paper."<<endl; 

//1000 trials take on the order of an hour or more for lattice sizes of 14x14x14 and bigger

//	unsigned int N = atoi(argv[1]);
//	unsigned int NY = atoi(argv[2]);
//	unsigned int NZ = atoi(argv[3]);
//	unsigned int SAMPLE = atoi(argv[4]);
//	double p = strtod(argv[5],0);
//	double Q = strtod(argv[6],0); //for us, input 1 for Q
//	cout << "# n=" << N << " # ny = "<<NY<<" # nz = "<<NZ<<" 	Samples = "<<SAMPLE<< " # p = "<<p<< " # q = "<<Q<<endl;
	
	double p = 0.70; //this is close to the percolation threshold for Q=1
	double Q = 1.0; //for us, Q = 1.0
	int SAMPLE = 1000;

ofstream simulation;
simulation.open ("simulationThresholdSizes1000TrialsFineScale.txt");

	bool Firstrun=false; //so setSeed for random number generation in pyrochlore constructor is only called the first time, not on every iteration

for (int size = 2; size <= 16; size=size+2)
{
	for (double prob=p-0.01; prob<=p+0.01 && prob<=1.; prob=prob+0.001)
	{

			cout<<"iteration prob ="<<prob<<endl; 
			unsigned ncrossed = 0;
			double psquared = prob*prob;
			double tempsize = 0;
			double nsize=0;//size of connected cluster from each successful crossing for a lattice is added to nsize. Hence, nsize divided by ncrossed gives the average size of 
//trees spanning NEAR to FAR vertices for a lattice.

//nsize/ncrossed is column not shown in the paper, and the usefulness of this number is not yet clear. This is the last column of the output text file.

			double latticesize=0;//size of underlying lattice, i.e. total number of qubits (some of which will not be part of the 
						//connected cluster)
			for (unsigned j=0;j<SAMPLE;j++)
			{	
			
			if (prob == p-0.15 && j==0 && size==2) 
			{	
				Firstrun=true; //setSeed is called in Pyrochlore constructor only when Firstrun = true.
								
			}
			else Firstrun = false; 
//create a pyrochlore lattice of size n * ny *nz, with each tetrahedron fully connected with probability = prob, and having two disconnected lines with probability = 1 - prob

//     		Pyrochlore PC(N, NY, NZ, prob, Firstrun);
			Pyrochlore PC(size, size, size, prob, Firstrun);

		if (j==0)
			latticesize=PC.size;
//clear vertices with probability = psquared, set input Q to 1, since we don't need to remove individual edges in our scheme.	  						  					
	    		PC.percolate(psquared, Q); 
	    		ncrossed += PC.is_crossable(&tempsize);
			nsize = nsize + tempsize;
			}
		if (ncrossed!=0)
			simulation<<size<<"\t"<<size<<"\t"<<size<<"\t"<<latticesize<<"\t"<<prob<<"\t"<<SAMPLE<<"\t"<<ncrossed<<"\t"<<floor(nsize/(double)ncrossed)<<"\n";
		else 
			simulation<<size<<"\t"<<size<<"\t"<<size<<"\t"<<latticesize<<"\t"<<prob<<"\t"<<SAMPLE<<"\t"<<ncrossed<<"\t"<<nsize<<"\n";
	}	 	
}
simulation.close();

}


void print_run_error(void)
{
	cerr << "Usage: main.exe N NY NZ Sample p q" << endl << endl << "where" << endl 
			<< "N, NY, NZ mean an NxNYxNZ pyrochlore lattice" << endl
			<< "Sample is the number of trials" << endl
	     <<"p is the success probability of each fusion measurement, and this code will scan probabilities from p-0.15 to p+0.15"<<endl
			<< "q is the probability of the edge being there, which is always 1 for our case" << endl;
}

