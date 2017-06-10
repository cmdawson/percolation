#include "unitcell.h"
#include <iostream>
#include <fstream>

#include <unordered_map>
#include <boost/numeric/ublas/io.hpp>

#include "coordinate.h"

using namespace std;
using namespace boost;
using namespace boost::numeric;

typedef color_traits<default_color_type> Color;

void save_unitcell();
void load_unitcell();

int main(int argc, char* argv[])
{	
	UnitCell A;

	ifstream inf;
	inf.open("tests/data/honeycomb.json");

	inf >> A;
	inf.close();

	UnitCell B = A;

	Graph::vertex_iter v0, v1;
    tie(v0,v1) = vertices(A.adjacency_list());
	while (v0 != v1)
	{
		property_tree::ptree node;
		A.adjacency_list()[*v0].x += A.axes()[0];
		//output_index[*v0] = counter++;
		//write_vertex(node, this->_adjacency_list[*v0++]);
		//vertex_list.push_back(make_pair("", node));
		cout << A.adjacency_list()[*v0].x << endl;
		v0++;
	}

	cout << "-----------------" << endl;

	tie(v0,v1) = vertices(B.adjacency_list());
	while (v0 != v1)
	{
		cout << B.adjacency_list()[*v0++].x << endl;
	}







	//save_unitcell();
	//load_unitcell();

	/*Graph::coordinate v(3), w(3);

	v[0] = 12.5;
	v[1] = -3.0;
	v[2] = 1.1;

	w[0] = 12.4;
	w[1] = -3.0;
	w[2] = 1.1;


	cout << v << endl;*/

	//cout << _hash(v) << endl;
	//cout << _hash(w) << endl;
	//cout << _equals(v,w) << endl;
	//cout << _equals2(v,v) << endl;

	/*cout << ublas::norm_1(v-w) << endl;

	std::unordered_map<std::reference_wrapper<Graph::coordinate>, int, coordinate_hash, coordinate_equals> umap(22);

	umap[w] = 10;
	umap[v] = 11;

	cout << umap[w] << endl;
	cout << umap[v] << endl;*/
}

void load_unitcell()
{
	UnitCell C;

	ifstream inf;
	inf.open("honeycomb.json");

	inf >> C;
	inf.close();

	Graph G = C;

	cout << G << endl;
	cout << C << endl;

	/*cout << C.axes().size() << endl;

	Graph::vertex_iter v0, v1;
    tie(v0,v1) = vertices(C.adjacency_list());
	while (v0 != v1)
	{
		Graph::Vertex& v =  C.adjacency_list()[*v0++];
		cout << v.x[0] << " " << v.x[1] << endl;
	}*/


	//cout << C;
}


void save_unitcell()
{
	UnitCell C;
	Graph::AdjacencyList& adjacency_list = C.adjacency_list();

	const int N=5;
	Graph::vertex_descriptor v[N];

	for (int i=0;i<N;i++)
	{
 		v[i] = add_vertex(adjacency_list);
		adjacency_list[v[i]].id = i;

		adjacency_list[v[i]].x = ublas::vector<double>(3);
		adjacency_list[v[i]].x[0] = i;
		adjacency_list[v[i]].x[1] = 2*i;
	}

	Graph::edge_descriptor e0;
	bool success;

	for (int i=0;i<N-1;i++)
	{
		tie(e0,success) = add_edge(v[i], v[i+1], adjacency_list);
		adjacency_list[e0].color = Color::green();
	}

	ofstream outf;
	outf.open("chain.json");
	outf << C;
	outf.close();
}


