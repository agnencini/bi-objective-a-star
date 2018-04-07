#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <boost/graph/adjacency_list.hpp>

struct vertex;
struct edge;
struct cost_set_ordering;

typedef std::pair<int, int> cost_pair;
typedef std::set<cost_pair, cost_set_ordering> cost_set;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, vertex, edge> graph;

struct vertex
{
	double lat, lon;
	std::string name;
};

struct edge
{
	cost_pair cost;
	int cost1, cost2;
	std::string name;
};

struct cost_set_ordering
{
	inline bool operator()(const cost_pair& a, const cost_pair& b) const
	{
		return (a.first < b.first);
	}
};

inline cost_pair operator+(const cost_pair& a, const cost_pair& b)
{
	return std::make_pair(a.first + b.first, a.second + b.second);
}

inline cost_pair operator-(const cost_pair& a, const cost_pair& b)
{
	return std::make_pair(a.first - b.first, a.second - b.second);
}

bool contains(const cost_pair& cost, const cost_set& s)
{
	cost_set::const_iterator ci = s.find(cost);
	if (ci == s.end())
		return false;
	else
		return (ci->second == cost.second);
}

#endif