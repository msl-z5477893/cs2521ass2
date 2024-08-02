
#include "DiGraph.h"
#include <stdbool.h>
#include <stdlib.h>

#include "AVLTreeInt.h"
#include "StackInt.h"

/* check for accessibility between computers for one direction.
 */
static bool directionalAccess(int securityFrom, int securityTo);

DiGraph DiGraphFromNetwork(int numComputers, int numConnections,
                           struct computer computers[],
                           struct connection connections[]) {
	DiGraph dgraph = malloc(sizeof(*dgraph));
	dgraph->vertices = numComputers;
	dgraph->adjacency = malloc(sizeof(*dgraph->adjacency) * dgraph->vertices);
	dgraph->adjCount = malloc(sizeof(*dgraph->adjCount) * dgraph->vertices);

	AVLTreeInt *adjTrees = malloc(sizeof(**adjTrees) * dgraph->vertices);
	for (int tree = 0; tree < dgraph->vertices; tree++) {
		adjTrees[tree] = AVLTreeIntNew();
	}
	for (int conn = 0; conn < numConnections; conn++) {
		int idCompA = connections[conn].computerA;
		int idCompB = connections[conn].computerB;
		int securityA = computers[idCompA].securityLevel;
		int securityB = computers[idCompB].securityLevel;
		if (directionalAccess(securityA, securityB)) {
			AVLTreeIntInsert(adjTrees[idCompA], idCompB);
		}
		if (directionalAccess(securityB, securityA)) {
			AVLTreeIntInsert(adjTrees[idCompB], idCompA);
		}
	}

	// turn each tree into list then delete each
	for (int tree = 0; tree < dgraph->vertices; tree++) {
		dgraph->adjacency[tree] = AVLTreeIntFlatten(adjTrees[tree]);
		dgraph->adjCount[tree] = adjTrees[tree]->count;
		AVLTreeIntFree(adjTrees[tree]);
	}
	free(adjTrees);

	return dgraph;
}

void DiGraphFree(DiGraph dg) {
	for (int adjIx = 0; adjIx < dg->vertices; adjIx++) {
		free(dg->adjacency[adjIx]);
	}
	free(dg->adjacency);
	free(dg->adjCount);
	free(dg);
}

struct nodeReachability *DiGraphNodeReachability(DiGraph graph, int nodeId) {
	AVLTreeInt visited = AVLTreeIntNew();
	StackInt stack = StackIntNew();

	// mark initial nodeId as visited
	StackIntPush(stack, nodeId);
	AVLTreeIntInsert(visited, nodeId);
	while (stack->count != 0) {
		int currNode = StackIntPop(stack);
		for (int adjIx = 0; adjIx < graph->adjCount[currNode]; adjIx++) {
			// if tree insertion is successful that means the node is newly visited
			// and we should check adjacencies for more accessible neighbours
			if (AVLTreeIntInsert(visited, graph->adjacency[currNode][adjIx])) {
				StackIntPush(stack, graph->adjacency[currNode][adjIx]);
			}
		}
	}

	struct nodeReachability *reachables = malloc(sizeof(*reachables));
	reachables->nodeSrc = nodeId;
	reachables->accessCount = visited->count;
	reachables->nodeAccessible = AVLTreeIntFlatten(visited);
	AVLTreeIntFree(visited);
	StackIntFree(stack);
	return reachables;
}

// LOCAL FUNCTIONS

static bool directionalAccess(int securityFrom, int securityTo) {
	return !(securityTo > securityFrom + 1);
}
