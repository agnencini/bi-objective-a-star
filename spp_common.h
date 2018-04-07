#pragma once

#ifndef SPP_COMMON_H
#define SPP_COMMON_H

#include <iostream>
#include "types.h"

bool merge(cost_set& dest, const cost_set& src, const cost_pair& edge_cost)
{
	bool merged = false;
	auto ii = src.begin();
	auto jj = dest.begin();

	while (ii != src.end() && jj != dest.end())
	{
		cost_pair sl = *ii + edge_cost;

		if (sl.first < jj->first)
		{
			bool deleted = false;
			while (jj != dest.end() && sl.second <= jj->second)
			{
				cost_set::iterator del = jj++;
				dest.erase(del);
				deleted = true;
			}

			dest.insert(sl);
			merged = true;
			++ii;
		}
		else if (sl.first > jj->first)
		{
			while (sl.second >= jj->second)
			{
				++ii;
				if (ii == src.end())
					break;
				sl = *(ii) + edge_cost;
			}
			++jj;
		}
		else
		{
			if (sl.second < jj->second)
			{
				bool deleted = false;
				while (jj != dest.end() && sl.second <= jj->second)
				{
					assert(sl.first <= jj->first);
					cost_set::iterator del = jj++;
					dest.erase(del);
					deleted = true;
				}

				dest.insert(sl);
				merged = true;
				++ii;
			}
			else if (sl.second > jj->second)
			{
				while (sl.second >= jj->second)
				{
					if (++ii == src.end()) 
						break;
					sl = *(ii) + edge_cost;
				}
				++jj;
			}
			else
			{
				++ii;
				++jj;
			}
		}
	}

	for (; ii != src.end(); ++ii)
	{
		cost_pair sl = *ii + edge_cost;
		dest.insert(sl);
		merged = true;
	}

	return merged;
}

bool over_target(const cost_set& target, const cost_set& src, const cost_pair& h)
{
	bool merged = false;
	auto ii = src.begin();
	auto jj = target.begin();

	while (ii != src.end() && jj != target.end())
	{
		bool deleted = false;
		if (ii->first + h.first < jj->first)
		{
			while (jj != target.end() && ii->second + h.second <= jj->second)
			{
				jj++;
				deleted = true;
			}

			merged = true;
			break;
			++ii;
		}
		else if (ii->first + h.first > jj->first)
		{
			while (ii->second + h.second >= jj->second)
			{
				assert(ii->first + h.first > jj->first);
				if (++ii == src.end())
					break;
			}
			++jj;
		}
		else
		{
			if (ii->second + h.second < jj->second)
			{
				bool deleted = false;
				while (jj != target.end() && ii->second + h.second <= jj->second)
				{
					assert(ii->first + h.first <= jj->first);
					jj++;
					deleted = true;
				}

				merged = true;
				break;
				++ii;
			}
			else if (ii->second + h.second > jj->second)
			{
				while (ii->second + h.second >= jj->second)
				{
					if (++ii == src.end())
						break;
				}
				++jj;
			}

			else
			{
				++ii;
				++jj;
			}
		}
	}

	for (; ii != src.end(); ++ii)
	{
		merged = true;
		break;
	}

	return !merged;
}

#endif