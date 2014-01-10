#include "pyrochlore.h"
#include "point3d.h"

using namespace std;
using namespace boost;


Lattice::Vertex add_point(point3d& p, Lattice::AdjacencyList& adj)
{
    typedef color_traits<default_color_type> Color;
    static int counter = 0;
    Lattice::Vertex v = add_vertex(adj);
    adj[v].x = p.x;
    adj[v].y = p.y;
    adj[v].z = p.z;
    adj[v].index = counter++;
    adj[v].color = Color::white();

    return v;
}


Pyrochlore::Pyrochlore(int n) : Lattice(n)
{
    point3d p0;
    Vertex v0;
    double l = 1.0;
    vector<Vertex> vlist;

    // Adding Hussain's bowtie 2
    p0.x = p0.y = p0.z = 0.0;
    vlist.push_back(add_point(p0,adjacency_list));

    p0.x = l;
    vlist.push_back(add_point(p0,adjacency_list));

    p0.x = p0.z = l/2.0;
    p0.y = -l/sqrt(2.0);
    vlist.push_back(add_point(p0,adjacency_list));

    p0.z = -l/2;
    vlist.push_back(add_point(p0,adjacency_list));

    p0.x = -l;
    p0.y = p0.z = 0.0;
    vlist.push_back(add_point(p0,adjacency_list));

    p0.x = p0.z = -l/2.0;
    p0.y = l/sqrt(2.0);
    vlist.push_back(add_point(p0,adjacency_list));
    
    p0.z = l/2.0;
    vlist.push_back(add_point(p0,adjacency_list));

    add_edge(vlist[0], vlist[1], adjacency_list);
    add_edge(vlist[0], vlist[2], adjacency_list);
    add_edge(vlist[0], vlist[3], adjacency_list);
    add_edge(vlist[0], vlist[4], adjacency_list);
    add_edge(vlist[0], vlist[5], adjacency_list);
    add_edge(vlist[0], vlist[6], adjacency_list);

    add_edge(vlist[1], vlist[2], adjacency_list);
    add_edge(vlist[1], vlist[3], adjacency_list);

    add_edge(vlist[2], vlist[3], adjacency_list);

    add_edge(vlist[4], vlist[5], adjacency_list);
    add_edge(vlist[4], vlist[6], adjacency_list);

    add_edge(vlist[5], vlist[6], adjacency_list);
}

void Pyrochlore::percolate(const double& p, const double& q)
{
}
