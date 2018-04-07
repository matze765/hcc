#!/usr/bin/env python3

# 2018 Matthias J. Kannwischer
# public domain


# libraries
import networkx as nx
import matplotlib.pyplot as plt 
import sys
from ast import literal_eval 

if len(sys.argv) != 2:
    print("Usage ./ploy.py path/to/file")
    exit(-1)

# parses an compiled flow graph output file 
def parseFile(path):
    with open(path) as f:
        lines = f.read().split("\n")
        f.close()
    edges = []
    nodes = []
    for node in lines:
        parts = node.split("\t")
        # ignore weird input (end of file?)
        if len(parts) < 3:
            continue
        nodeId = int(parts[0])
        nodeType = parts[1]
        nodes.append((nodeId, nodeType))
        # C nodes may have arbitrarly many output edges  
        if nodeType == "C":
            for n in parts[2:]:
                otherNodeId = int(literal_eval(n)[0])
                edges.append((nodeId, otherNodeId))
        else:
            leftOutput = parts[2]
            rightOutput = parts[3]
            if leftOutput != "-":
                otherNodeId = int(literal_eval(leftOutput)[0])
                edges.append((nodeId, otherNodeId))
            if rightOutput != "-":
                otherNodeId = int(literal_eval(rightOutput)[0])
                edges.append((nodeId, otherNodeId))
    return dict(nodes), edges


# parse the files
nodes, edges = parseFile(sys.argv[1])

print(nodes)
print(edges)
edges_renamed = []
# rename the nodes in the edges to include the node type
for edge in edges:
    left = edge[0]
    right = edge[1]
    left_renamed = "{}-{}".format(nodes[left], left)
    right_renamed = "{}-{}".format(nodes[right], right);
    edges_renamed.append((left_renamed, right_renamed, 1))

print(edges_renamed)



DG = nx.DiGraph()
DG.add_weighted_edges_from(edges_renamed)
#DG.out_degree(1, weight='weight')
#DG.degree(1, weight='weight')

options = {
    'node_size': 900
}
 
# Plot it
nx.draw_kamada_kawai(DG, with_labels=True, **options)
plt.show()

