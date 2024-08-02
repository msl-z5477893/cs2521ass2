/* A graph representing all the details of the network.
 * Including the cost of each edge and node. 
 */
#ifndef Fullgraph_h_INCLUDED
#define Fullgraph_h_INCLUDED

#include <stdbool.h>

#include "poodle.h"

struct graph {
	int vertices;
	int *vertexCost;
	int **edges;
	int *edgeCount;
};

struct djikstraData {
	int vertices;
	int visits;
	int *distance;
	int *predecessor;
};

typedef struct graph *Fullgraph;

/* Generate graph from provided network details.
 */
Fullgraph FullgraphGenerate(int numComps, int numConns, struct computer comps[],
                            struct connection conns[]);

/* Free graph */
void FullgraphFree(Fullgraph graph);

/* Given the graph and a starting node, ind the path to every node in
 * graph (if possible) with djikstra algorithm.
 */
struct djikstraData *FullgraphDjikstra(Fullgraph g, int vertexSrc);

#endif  // Fullgraph_h_INCLUDED
