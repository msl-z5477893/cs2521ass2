/* A digraph for representing the network.
 */
#ifndef DiGraph_h_INCLUDED
#define DiGraph_h_INCLUDED

#include "poodle.h"

struct digraph {
	int vertices;
	int **adjacency;
};

typedef struct digraph *DiGraph;

DiGraph DiGraphFromNetwork(int numComputers, int numConnections,
                           struct computer computers[],
                           struct connection connections[]);

void DiGraphFree(DiGraph dgraph);

#endif  // DiGraph_h_INCLUDED
