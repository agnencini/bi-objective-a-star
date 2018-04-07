#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <boost/tokenizer.hpp>
#include "types.h"

void load_graph(std::istream& in, graph& g)
{
	int num_vertices, num_edges;
	in >> num_vertices >> num_edges;

	std::string dummy;
	std::getline(in, dummy);

	graph new_graph(num_vertices);
	g = new_graph;

	for (int u = 0; u < num_vertices; ++u)
	{
		std::string line;
		std::getline(in, line);

		boost::char_separator<char> separator(" ");
		boost::tokenizer< boost::char_separator<char> > tokenizer(line, separator);

		auto it = tokenizer.begin();
		g[u].lat = std::stod(*it++);
		g[u].lon = std::stod(*it++);
		g[u].name = *it;
	}

	for (int u = 0; u < num_edges; ++u)
	{
		std::string line;
		std::getline(in, line);

		boost::char_separator<char> separator(" ");
		boost::tokenizer< boost::char_separator<char> > tokenizer(line, separator);

		auto it = tokenizer.begin();
		int from = std::stoi(*it++);
		int to = std::stoi(*it++);
		int cost1 = std::stoi(*it++);
		std::string cycleway = *it++;
		std::string oneway = *it++;
		std::string name = "";

		for (it; it != tokenizer.end(); ++it)
			name = name + *it + " ";

		// No self-loops allowed
		if (from == to)
			continue;

		// No free-cost edges allowed
		if (cost1 == 0)
			cost1 = 1;

		int cost2 = 0;
		if (cycleway[0] == 'n')
			cost2 = cost1;

		graph::edge_descriptor e;
		bool added;
		tie(e, added) = add_edge(from, to, g);
		if (added)
		{
			g[e].cost.first = cost1;
			g[e].cost.second = cost2;
			g[e].cost1 = g[e].cost.first;
			g[e].cost2 = g[e].cost.second;
			g[e].name = name;
		}

		if (oneway[0] == 'n')
		{
			tie(e, added) = add_edge(to, from, g);
			if (added)
			{
				g[e].cost.first = cost1;
				g[e].cost.second = cost2;
				g[e].cost1 = g[e].cost.first;
				g[e].cost2 = g[e].cost.second;
				g[e].name = name;
			}
		}
	}
}

#endif