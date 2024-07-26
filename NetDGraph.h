/* A graph data structure representing an unweighted, undirected graph of a network
 * that is based on provided network details i.e.,
 * computers and connections.
 */

#ifndef NetDGraph_h_INCLUDED
#define NetDGraph_h_INCLUDED

#include "IntSet.h"
#include "poodle.h"

/* The NetDGraph structure contains the following,
 * - the number of computers (int)
 * - the security levels of each computer (int *)
 * - the adjacency list of each computer (IntSet)
 * Computers have IDs ranging between 0 and <no. of computers> - 1.
 */
typedef struct {
	int computers;
	IntSet *adjSet;
} *NetDGraph;

/* Generate NetDGraph from provided network details.
 */
NetDGraph NetDGraphGenerate(struct computer computers[], int numComputers,
                            struct connection connections[],
                            int numConnections);

/* free NetDGraph from memory
 */
void NetDGraphFree(NetDGraph ng);

#endif  // NetDGraph_h_INCLUDED
