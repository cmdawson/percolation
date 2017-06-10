#include "graph.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;
using namespace boost::numeric;

typedef color_traits<default_color_type> Color;


void save_graph();
void load_graph();

int main(int argc, char* argv[])
{	
	//save_graph();
	load_graph();
}

void load_graph()
{
	Graph G;

	ifstream inf;
	inf.open("chain.json");

	inf >> G;
	inf.close();

	cout << G;
}


void save_graph()
{
	Graph G;
	Graph::AdjacencyList& adjacency_list = G.get_adjacency_list();

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
	outf << G;
	outf.close();
}


