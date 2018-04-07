#pragma once

#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <vector>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

std::vector<cost_pair> h12(const graph& g, const int goal)
{
	int num_vertices = boost::num_vertices(g);
	boost::reverse_graph<graph> g_reversed = boost::make_reverse_graph(g);

	std::vector<cost_pair> h(num_vertices);
	std::vector<int> dist(num_vertices);
	std::vector<graph::vertex_descriptor> prev(num_vertices);

	boost::dijkstra_shortest_paths(g_reversed,
		(graph::vertex_descriptor)goal,
		weight_map(boost::get(&edge::cost1, g_reversed))
		.predecessor_map(&prev[0])
		.distance_map(&dist[0]));

	for (int u = 0; u < num_vertices; ++u)
		h[u].first = dist[u];

	boost::dijkstra_shortest_paths(g_reversed,
		(graph::vertex_descriptor)goal,
		weight_map(boost::get(&edge::cost2, g_reversed))
		.predecessor_map(&prev[0])
		.distance_map(&dist[0]));

	for (int u = 0; u < num_vertices; ++u)
		h[u].second = dist[u];

	return h;
}

#endif