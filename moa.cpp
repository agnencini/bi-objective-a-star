#include <boost/algorithm/string.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "spp_common.h"
#include "utils.h"
#include "tracking.h"
#include "heuristics.h"

using namespace std;

vector<cost_set> moa(graph& g, unsigned int s, unsigned int goal, vector<cost_pair>& H)
{
	int n = boost::num_vertices(g);

	vector<cost_set> G(n);
	G[s].insert(make_pair(0, 0));

	vector<unsigned int> open;
	open.push_back(s);

	vector<bool> is_open(n, false);
	is_open[s] = true;

	while (!open.empty())
	{
		cost_pair best = *(G[open[0]].begin()) + H[open[0]];
		int best_index = 0;

		for (unsigned int u = 1; u < open.size(); ++u)
		{
			cost_pair c = *(G[open[u]].begin()) + H[open[u]];
			if (c.first < best.first || (c.first == best.first && c.second < best.second))
			{
				best = c;
				best_index = u;
			}
		}

		unsigned int i = open[best_index];
		open.erase(open.begin() + best_index);
		is_open[i] = false;

		graph::out_edge_iterator ei, ei_end;
		for (boost::tie(ei, ei_end) = boost::out_edges(i, g); ei != ei_end; ++ei)
		{
			auto j = boost::target(*ei, g);
			bool changed = merge(G[j], G[i], g[*ei].cost);

			if (changed && !is_open[j] && j != goal)
			{
				if (!over_target(G[goal], G[j], H[j]))
				{
					open.push_back(j);
					is_open[j] = true;
				}
			}
		}
	}

	return G;
}

// SOLVE PROBLEM: moa [graph_file] [start] [goal] [kml_file]
int main(int argc, char* argv[])
{
	graph g;
	ifstream in(argv[1]);
	load_graph(in, g);

	unsigned int start = atoi(argv[2]);
	unsigned int goal = atoi(argv[3]);

	vector<cost_pair> H = h12(g, goal);
	vector<cost_set> G = moa(g, start, goal, H);

	ofstream out(argv[4]);
	export_kml(out, g, G, start, goal);

	return 0;
}