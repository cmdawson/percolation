#include <boost/graph/undirected_dfs.hpp>

namespace boost {

template <typename Graph, typename DFSVisitor, typename VertexColorMap,
	typename EdgeColorMap, typename Vertex> void undirected_traverse(
		const Graph& g, DFSVisitor vis, VertexColorMap vertex_color,
		EdgeColorMap edge_color, Vertex start_vertex)
{
	typedef typename property_traits<VertexColorMap>::value_type ColorValue;
	typedef color_traits<ColorValue> Color;

	typename graph_traits<Graph>::vertex_iterator ui, ui_end;
	for (boost::tie(ui, ui_end) = vertices(g); ui != ui_end; ++ui)
	{
		put(vertex_color, *ui, Color::white());
		vis.initialize_vertex(*ui, g);
	}
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
	{
		put(edge_color, *ei, Color::white());
	}

	vis.start_vertex(start_vertex, g);
	detail::undir_dfv_impl(g, start_vertex, vis, vertex_color, edge_color);
}

}

