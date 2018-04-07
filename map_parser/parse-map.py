#!/usr/bin/env python

import sys
import math
import codecs
from xml.parsers import expat

if len(sys.argv) < 2:
    print("Usage: parse-map.py [osm_file] [output_file]")
    sys.exit(0)

DEG_LEN = 111000.0
inside_way = False
current_way = None

ways = {}
nodes = []

def start_element(name, attrs):
    global current_way, inside_way
    if name == u"node":
        nodes.append({"id": attrs["id"], "lon": attrs["lon"], "lat": attrs["lat"]})
    elif name == u"way":
        inside_way = True
        current_way = {"name": "No Name", "nodes": [], "street": None, "cycleway": None, "oneway": None}
        ways[attrs["id"]] = current_way
    elif inside_way and name == u"nd":
        current_way["nodes"].append(attrs["ref"])
    elif inside_way and name == u"tag" and attrs["k"] == u"name":
        current_way["name"] = attrs["v"]
    elif inside_way and name == u"tag" and attrs["k"] == u"highway":
        current_way["street"] = attrs["v"]
    elif inside_way and name == u"tag" and attrs["k"] == u"cycleway":
        current_way["cycleway"] = attrs["v"]
    elif inside_way and name == u"tag" and attrs["k"] == u"oneway":
        current_way["oneway"] = attrs["v"]

def distance(dlat1, dlat2, dlon1, dlon2):

    lat1 = float(dlat1)
    lat2 = float(dlat2)
    lon1 = float(dlon1)
    lon2 = float(dlon2)

    x = lat1 - lat2
    y = (lon1 - lon2) * math.cos(math.radians(lat2))

    dist = DEG_LEN * math.sqrt(x * x + y * y)
    return int(round(dist))

def end_element(name):
    global current_way, inside_way
    if name == u"way":
        inside_way = False
        current_way = None

p = expat.ParserCreate("utf-8")

p.StartElementHandler = start_element
p.EndElementHandler = end_element

theMap = open(sys.argv[1], "rb")
p.ParseFile(theMap)

validNodes = set()
edges = []
for key in ways.keys():
    way = ways[key]
    if way["street"] or way["cycleway"]:
        for i in range(1, len(way["nodes"])):
            edges.append({"name": way["name"], "origin": way["nodes"][i - 1], "destination": way["nodes"][i],
                          "cycleway": (way["cycleway"] is not None and (
                          way["cycleway"] == "yes" or way["cycleway"] == "track" or way["cycleway"] == "lane")),
                          "oneway": (way["oneway"] is not None and way["oneway"] == "yes")})
        for node in way["nodes"]:
            validNodes.add(node)

out = codecs.open(sys.argv[2], "w", "utf-8")

num_nodes = 0;
for node in nodes:
    if node["id"] in validNodes:
        num_nodes = num_nodes + 1

out.write(str(num_nodes) + " ")
out.write(str(len(edges)) + "\n")

dict = {}
dict2 = {}
counter = 0
counter2 = 0
for node in nodes:
    if node["id"] in validNodes:
        out.write(str(node["lat"]) + " " + str(node["lon"]) + " " + str(node["id"]) + "\n")
        dict[node["id"]] = counter2
        dict2[node["id"]] = counter
        counter = counter + 1
    counter2 = counter2 + 1

for edge in edges:
    dist = distance(nodes[dict[edge["origin"]]]["lat"], nodes[dict[edge["destination"]]]["lat"],
                    nodes[dict[edge["origin"]]]["lon"], nodes[dict[edge["destination"]]]["lon"])
    out.write(str(dict2[edge["origin"]]) + " " + str(dict2[edge["destination"]]) + " " + str(dist) + " " +
              ("y" if edge["cycleway"] else "n") + " " + ("y" if edge["oneway"] else "n") + " " + edge["name"] + "\n")

out.close()


