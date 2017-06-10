#include "graph.h"
#include <map>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional/optional.hpp>
#include <iostream>

using namespace std;
using namespace boost;
namespace pt = boost::property_tree;

typedef color_traits<default_color_type> Color;

void read_vertex(const property_tree::ptree& node, Graph::Vertex& v)
{
	optional<const pt::ptree&> posx = node.get_child_optional("x");
	if (posx)
	{
		v.x = Graph::coordinate(posx->count(""));
		unsigned i = 0;
		for (const pt::ptree::value_type& ex : *posx)
		{
			v.x[i++] = ex.second.get_value<double>();
		}
	}
	v.id = node.get<int>("id");
	v.color = static_cast<default_color_type>(node.get<int>("c", 0));
}

void write_vertex(property_tree::ptree& node, const Graph::Vertex& v)
{
	if (v.x.size() > 0)
	{
		property_tree::ptree px;
		for (auto ex=v.x.begin(); ex!=v.x.end(); ex++)
		{
			property_tree::ptree enode;
			enode.put_value(*ex);
			px.push_back(std::make_pair("",enode));
		}
		node.add_child("x", px);
	}

	node.put("id", v.id);
	node.put("c", static_cast<int>(v.color));
}

void read_edge(const property_tree::ptree& node, Graph::Edge& e)
{
	e.color = static_cast<default_color_type>(node.get<int>("c", 0));
}

void write_edge(property_tree::ptree& node, const Graph::Edge& e)
{
	node.put("c", static_cast<int>(e.color));
}

void Graph::read_from_ptree(const property_tree::ptree& root)
{
	map<int,vertex_descriptor> vertex_map;

	for (const property_tree::ptree::value_type &node : root.get_child("vertices"))
	{
		vertex_descriptor v = add_vertex(this->_adjacency_list);
		read_vertex(node.second, this->_adjacency_list[v]);
		vertex_map[this->_adjacency_list[v].id] = v;
	}

	for (const property_tree::ptree::value_type &node : root.get_child("edges"))
	{
		edge_descriptor e0;
		bool success;

		unsigned src = node.second.get<unsigned>("src");
		unsigned tgt = node.second.get<unsigned>("tgt");

		tie(e0,success) = add_edge(vertex_map[src], vertex_map[tgt], this->_adjacency_list);
		read_edge(node.second, this->_adjacency_list[e0]);
	}
}


istream& operator>>(istream &input, Graph &G)
{
	property_tree::ptree root;
	property_tree::read_json(input, root);
	G.read_from_ptree(root);
	return input;
}


void Graph::write_to_ptree(property_tree::ptree& root) const
{
    map<vertex_descriptor, unsigned> output_index;
	property_tree::ptree vertex_list, edge_list;

	unsigned counter = 0;
    vertex_iter v0, v1;
    tie(v0,v1) = vertices(this->_adjacency_list);
	while (v0 != v1)
	{
		property_tree::ptree node;
		output_index[*v0] = counter++;
		write_vertex(node, this->_adjacency_list[*v0++]);
		vertex_list.push_back(make_pair("", node));
	}

	edge_iter e0, e1;
    tie(e0,e1) = edges(this->_adjacency_list);
	while (e0 != e1)
	{
		property_tree::ptree node;
		node.put("src", output_index[source(*e0,this->_adjacency_list)]);
		node.put("tgt", output_index[target(*e0,this->_adjacency_list)]);
		write_edge(node, this->_adjacency_list[*e0++]);
		edge_list.push_back(make_pair("", node));
	}

	root.add_child("vertices", vertex_list);
	root.add_child("edges", edge_list);
}


ostream &operator<<(ostream &output, const Graph& G)
{
	property_tree::ptree root;
	G.write_to_ptree(root);
	write_json(output, root, false);
	return output;
}

/*
	
    graph_traits<Graph::AdjacencyList>::edge_iterator e0, e1;
    tie(e0,e1) = edges(H.adjacency_list);

    if (e0 != e1)
    {
		unsigned src = oidx[source(*e0,H.adjacency_list)];
		unsigned tgt = oidx[target(*e0,H.adjacency_list)];
		output << "[" << src << "," << tgt << "]";
    }
    while (++e0 != e1)
    {
		unsigned src = oidx[source(*e0,H.adjacency_list)];
		unsigned tgt = oidx[target(*e0,H.adjacency_list)];
		output << ",[" << src << "," << tgt << "]";
    }
    output << "]}";
	*/


    /*  MDL mol format version
    const char* vsuffix = " C   0  0  0  0  0  0  0  0  0  0  0  0";
    const char* esuffix = "  1  0  0  0  0";
    format vformat("%1$10.4f%2$10.4f%3$10.4f"); // 3 floats, width=10
    format eformat("%1$3d%2$3d"); // 2 ints, width=3
    map<Graph::Vertex, unsigned> oidx;
    unsigned ii = 0;

    if (num_vertices(H.adjacency_list) > 999)
	throw runtime_error("MOL files can only cope with <999 vertices");

    output << "percolatedlattice" << endl;
    output << " PERC/whatever101" << endl << endl;
    output << eformat % num_vertices(H.adjacency_list) 
		% num_edges(H.adjacency_list) 
	<< "  0  0  0  0  0  0  0  0  1 V2000" << endl;

    Graph::vertex_iter v0, v1;
    tie(v0,v1) = vertices(H.adjacency_list);
    while (v0 != v1)
    {
	output << vformat % H.adjacency_list[*v0].x 
		    % H.adjacency_list[*v0].y 
		    % H.adjacency_list[*v0].z 
	    << vsuffix << endl;
	oidx[*v0] = ii++;
	++v0;
    }
    
    graph_traits<Graph::AdjacencyList>::edge_iterator e0, e1;
    tie(e0,e1) = edges(H.adjacency_list);
    while (e0 != e1)
    {
	//unsigned src = H.adjacency_list[source(*e0,H.adjacency_list)].index;
	//unsigned tgt = H.adjacency_list[target(*e0,H.adjacency_list)].index;
	unsigned src = oidx[source(*e0,H.adjacency_list)] + 1;
	unsigned tgt = oidx[target(*e0,H.adjacency_list)] + 1;
	output << eformat % src % tgt << esuffix << endl;
	++e0;
    }

    output << "M  END";
    
    return output; */
