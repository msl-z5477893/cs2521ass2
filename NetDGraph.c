#include "NetDGraph.h"
#include <stdlib.h>

#include "IntPq.h"
#include "IntSet.h"
#include "poodle.h"

static bool networkAccessibleFromDir(int securityLevelFrom,
                                     int securityLevelTo);

NetDGraph NetDGraphGenerate(struct computer computers[], int numComputers,
                            struct connection *connections,
                            int numConnections) {
	// return NULL;
	// init main structure.
	NetDGraph dg = malloc(sizeof(*dg));
	dg->computers = numComputers;

	// init adjacency sets in each structure.
	dg->adjSet = malloc(sizeof(**dg->adjSet) * dg->computers);
	for (int set = 0; set < dg->computers; set++) {
		dg->adjSet[set] = IntSetNew();
	}

	// init a queue for each.
	IntPq *pqEach = malloc(sizeof(**pqEach) * dg->computers);
	for (int queue = 0; queue < dg->computers; queue++) {
		pqEach[queue] = IntPqNew();
	}

	// check for valid connections, putting them on proper queues.
	for (int conn = 0; conn < numConnections; conn++) {
		int idCompA = connections[conn].computerA;
		int idCompB = connections[conn].computerB;
		int secLvlA = computers[idCompA].securityLevel;
		int secLvlB = computers[idCompB].securityLevel;
		if (networkAccessibleFromDir(secLvlA, secLvlB)) {
			IntPqPush(pqEach[idCompA], idCompB);
		}
		if (networkAccessibleFromDir(secLvlB, secLvlA)) {
			IntPqPush(pqEach[idCompB], idCompA);
		}
	}

	// dump queue onto set, set must be in ascending order.
	for (int currQ = 0; currQ < dg->computers; currQ++) {
		while (pqEach[currQ]->count != 0) {
			int addedNum = IntPqPop(pqEach[currQ]);
			IntSetAdd(dg->adjSet[currQ], addedNum);
		}
	}

	// IMPORTANT! Write cleanup code here.
	for (int qCount = 0; qCount < dg->computers; qCount++) {
		IntPqFree(pqEach[qCount]);
	}
	free(pqEach);

	return dg;
}

void NetDGraphFree(NetDGraph dg) {
	for (int set = 0; set < dg->computers; set++) {
		IntSetFree(dg->adjSet[set]);
	}
	free(dg->adjSet);
	free(dg);
}

/* check if you can go between computers A to B in a certain direction
 * based the security level of source computer to destination computer.
 * The logic is based on task 1, when we had to return a NO_PERMISSION
 * if accessibility is invalid.
 */
static bool networkAccessibleFromDir(int securityLevelFrom,
                                     int securityLevelTo) {
	return !(securityLevelTo > securityLevelFrom + 1);
}
