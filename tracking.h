#pragma once

#ifndef TRACKING_H
#define TRACKING_H

#include <iostream>
#include "types.h"

std::vector< std::vector<unsigned int> > get_paths(graph& g, std::vector<cost_set>& G, unsigned int s, unsigned int goal)
{
	std::vector< std::vector<unsigned int> > solutions;

	for (auto csi = G[goal].begin(); csi != G[goal].end(); ++csi)
	{
		std::vector<unsigned int> solution;
		cost_pair lbl = *csi;
		unsigned int node = goal;
		solution.insert(solution.begin(), node);

		while (node != s)
		{
			graph::in_edge_iterator ei, ei_end;
			for (tie(ei, ei_end) = boost::in_edges(node, g); ei != ei_end; ++ei)
			{
				auto u = source(*ei, g);
				if (contains(lbl - g[*ei].cost, G[u]))
				{
					node = u;
					solution.insert(solution.begin(), node);
					lbl = lbl - g[*ei].cost;
					break;
				}
			}
		}
		solutions.push_back(solution);
	}
	return solutions;
}

void export_kml(std::ostream& out, graph& g, std::vector<cost_set>& G, unsigned int s, unsigned int goal)
{
	std::string tabs[] = { "", "\t", "\t\t", "\t\t\t", "\t\t\t\t", "\t\t\t\t\t" };

	std::vector< std::vector<unsigned int> > paths = get_paths(g, G, s, goal);
	std::string title = "Optimal paths from " + g[s].name + " to " + g[goal].name;

	out << tabs[0] << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl 
		<< tabs[0] << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">" << std::endl
		<< tabs[1] << "<Document>" << std::endl
		<< tabs[2] << "<name>" << title << "</name>" << std::endl 
		<< std::endl
		<< tabs[2] << "<Style id=\"pathColor\">" << std::endl
		<< tabs[3] << "<LineStyle>" << std::endl
		<< tabs[4] << "<color>ffffffff</color>" << std::endl
		<< tabs[4] << "<width>4</width>" << std::endl
		<< tabs[3] << "</LineStyle>" << std::endl
		<< tabs[3] << "<PolyStyle>" << std::endl
		<< tabs[4] << "<color>ffffffff</color>" << std::endl
		<< tabs[3] << "</PolyStyle>" << std::endl
		<< tabs[2] << "</Style>" << std::endl
		<< std::endl;

	cost_set::const_iterator it = G[goal].begin();
	for (unsigned int u = 0; u < paths.size(); ++u)
	{
		out << tabs[2] << "<Placemark>" << std::endl
		<< tabs[3] << "<name>" << "(" << it->first << ", " << it->second << ")" << "</name>" << std::endl
		<< tabs[3] << "<styleUrl>#pathColor</styleUrl>" << std::endl
		<< tabs[3] << "<LineString>" << std::endl
		<< tabs[4] << "<tessellate>1</tessellate>" << std::endl
		<< tabs[4] << "<altitudeMode>clampToGround</altitudeMode>" << std::endl
		<< tabs[4] << "<coordinates>" << std::endl;

		for (unsigned int v = 0; v < paths[u].size(); ++v)
			out << tabs[5] << g[paths[u][v]].lon << "," << g[paths[u][v]].lat << std::endl;

		out	<< tabs[4] << "</coordinates>" << std::endl
			<< tabs[3] << "</LineString>" << std::endl
			<< tabs[2] << "</Placemark>" << std::endl
			<< std::endl;
		++it;
	}
	out << tabs[1] << "</Document>" << std::endl
		<< tabs[0] << "</kml>" << std::endl;
}

#endif