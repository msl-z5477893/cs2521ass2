// DiGraph source goes here

#include "DiGraph.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "AVLTreeInt.h"

/* check for accessibility between computers for one direction.
 */
static bool directionalAccess(int securityFrom, int securityTo);

DiGraph DiGraphFromNetwork(int numComputers, int numConnections,
                           struct computer computers[],
                           struct connection connections[]) {
	// return NULL;
	DiGraph dgraph = malloc(sizeof(*dgraph));
	dgraph->vertices = numComputers;
	dgraph->adjacency = malloc(sizeof(*dgraph->adjacency) * dgraph->vertices);
	// TODO: loop through network data and build adjacency list.

	AVLTreeInt *adjTrees = malloc(sizeof(**adjTrees) * dgraph->vertices);
	for (int tree = 0; tree < dgraph->vertices; tree++) {
		adjTrees[tree] = AVLTreeIntNew();
	}
	for (int conn = 0; conn < numConnections; conn++) {
		// GO THROUGH EACH CONNECTION IN ARRAY AND CHECK BOTH SIDES.
		int idCompA = connections[conn].computerA;
		int idCompB = connections[conn].computerB;
		int securityA = computers[idCompA].securityLevel;
		int securityB = computers[idCompB].securityLevel;
		// DEBUG CODE:
		// printf("DiGraph.c line 33: ");
		// printf("Testing connection: (id %d sec %d)==(id %d sec %d)\n", idCompA,
		//        securityA, idCompB, securityB);
		if (directionalAccess(securityA, securityB)) {
			// DEBUG CODE:
			// printf("computer %d accessible from computer %d\n", idCompA,
			//        idCompB);
			// bool inputResult = AVLTreeIntInsert(adjTrees[idCompA], idCompB);
			AVLTreeIntInsert(adjTrees[idCompA], idCompB);
			// DEBUG CODE:
			// printf("AVL insertion status: %s\n",
			//        inputResult ? "true" : "false");
		}
		if (directionalAccess(securityB, securityA)) {
			// DEBUG CODE:
			// printf("computer %d accessible from computer %d\n", idCompB,
			//        idCompA);
			// bool inputResult = AVLTreeIntInsert(adjTrees[idCompB], idCompA);
			AVLTreeIntInsert(adjTrees[idCompB], idCompA);
			// DEBUG CODE:
			// printf("AVL insertion status: %s\n",
			//        inputResult ? "true" : "false");
		}
	}

	// turn each tree into list then delete each
	for (int tree = 0; tree < dgraph->vertices; tree++) {
		dgraph->adjacency[tree] = AVLTreeIntFlatten(adjTrees[tree]);
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
	free(dg);
}

static bool directionalAccess(int securityFrom, int securityTo) {
	return !(securityTo > securityFrom + 1);
}
