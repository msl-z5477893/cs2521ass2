/* A graph representing all the details of the network.
 * Including the cost of each edge and node. 
 */
#ifndef Fullgraph_h_INCLUDED
#define Fullgraph_h_INCLUDED

#include "poodle.h"

struct graph {
	int vertices;
	int *vertexCost;
	int **edges;
	int *edgeCount;
};

typedef struct graph *Fullgraph;

/* Generate graph from provided network details.
 */
Fullgraph FullgraphGenerate(int numComps, int numConns, struct computer comps[],
                            struct connection conns[]);

/* Free graph */
void FullgraphFree(Fullgraph graph);

// struct step *FullgraphTraverse();

#endif  // Fullgraph_h_INCLUDED
