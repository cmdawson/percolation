percolation
===========

Calculation of percolation thresholds for 2d lattices for Tez. Makes use of the boost::graph library.

The main.cpp file shows the very basic usage. Also included is the usual ostream::operator<< overload which outputs a description of the lattice that can be visualized using d3/lattice.html. They look pretty and are sort of useful for ensuring the lattice is what you think it is.

The percolated honeycomb lattices considered here correspond to certain quantum 'cluster' states prepared using optical bell pairs and fusion gates. 

For background on such thresholds see for example <a href="http://en.wikipedia.org/wiki/Percolation_threshold">here</a> and see <a href="http://arxiv.org/abs/quant-ph/0605014">quant-ph/0605014</a> for their significance in quantum computing.
