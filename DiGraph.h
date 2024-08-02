/* A digraph for a simple, limited representation of a network.
 * Only shows the connection between nodes.
 */
#ifndef DiGraph_h_INCLUDED
#define DiGraph_h_INCLUDED

#include "poodle.h"

struct digraph {
	int vertices;
	int **adjacency;
	int *adjCount;
};

struct nodeReachability {
	int nodeSrc;
	int *nodeAccessible;
	int accessCount;
};

typedef struct digraph *DiGraph;

/*
 * Create a digraph based on the following data
 * - a list of computers
 * - a list of connections of the given computers
 * - number of computers in the list
 * - numver of connections in the list
 */
DiGraph DiGraphFromNetwork(int numComputers, int numConnections,
                           struct computer computers[],
                           struct connection connections[]);

/* from a node with ID n return an array of nodes with id n that
 * are accessible from source node and the number of these
 */
struct nodeReachability *DiGraphNodeReachability(DiGraph graph, int nodeId);

void DiGraphFree(DiGraph dgraph);

#endif  // DiGraph_h_INCLUDED
