# bi-objective-a-star
An extension of A* for bi-objective shortest path problems

A* is a well-known algorithm for univariate shortest path problem (SPP); its efficiency over other algoritms like Dijkstra or Bellman-Ford is given by the use of heuristic functions to estimate solutions' cost. MOA* is an extension of A* for double-weighted graphs that finds all the pareto-efficient solutions. Multi-objective SPP has exponential complexity, however MOA* shows great performance improvements over uninformed algorithms like Brumbaugh-Smith.  

I made a Python script that converts OpenStreetMap .osm maps to a simple graph format; the edge weights are:  
1) The length of the edge (street length)
2) The length of the street without bycicle lane
  
So, for example, a street of length L without cycle lane has cost (L, L), instead a fully cyclable street has cost (L, 0). MOA* finds the shortest path, the most cyclable one, and all the others pareto-efficient solutions between them. The paths are then exported to a KML file, so they can be viewed with Google Earth or JOSM.
