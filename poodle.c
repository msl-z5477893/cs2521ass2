// poodle.c
// Assignment 2 main source file.
// completed by Michael Stephen Lape (z5477893)

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Bookmark.h"
#include "DiGraph.h"
#include "Fullgraph.h"
#include "PqNeighbours.h"
#include "poodle.h"

// LOCAL FUNCTIONS.
static int *copyArrayInt(int *arr, int arrSize);
static struct step *processDjikstraData(struct djikstraData *data);

static struct computerList *addComputer(struct computerList *list,
                                        int computer);

////////////////////////////////////////////////////////////////////////
// Task 1

/*
 * given a path of computers, assess that it works and the time taken
 * to probe it
 *
 * STATUS: UNOPTIMISED (slowed on challenge tasks)
 *
 * - Fails challenge test for the following reason:
 * - current computer and connection lookup time is O(n), which is very slow
 *   for challenge times
 */
struct probePathResult probePath(struct computer computers[], int numComputers,
                                 struct connection connections[],
                                 int numConnections, int path[],
                                 int pathLength) {
	struct probePathResult res = {SUCCESS, 0};
	Bookmark vists = BookmarkInit(numComputers);

	// keep track of the final computer reached.
	int finalComp = path[0];
	for (int ix = 0; ix < pathLength - 1; ix++) {
		int compFrom = path[ix];
		int compTo = path[ix + 1];
		if (compFrom == compTo) {
			continue;
		}
		struct connection *currentConn = NULL;
		// check if connection exists between chosen computers.
		for (int connIx = 0; connIx < numConnections; connIx++) {
			bool fSide = (connections[connIx].computerA == compFrom &&
			              connections[connIx].computerB == compTo);
			bool bSide = (connections[connIx].computerB == compFrom &&
			              connections[connIx].computerA == compTo);
			if (fSide || bSide) {
				currentConn = &connections[connIx];
				break;
			}
		}

		if (currentConn == NULL) {
			// fprintf(stderr, "Cannot find path: %d -> %d\n", compFrom, compTo);
			res.status = NO_CONNECTION;
			break;
		}

		// source computer with security level X can sand data to computers with
		// security level X + 1 or smaller
		// so to check for invalidity we just check if dest computer has security
		// level is strictly higher than the security of the source computer + 1.
		if (computers[compTo].securityLevel >
		    computers[compFrom].securityLevel + 1) {
			res.status = NO_PERMISSION;
			break;
		}

		// if current path is valid then we can count the time to the total
		if (BookmarkMark(vists, compFrom)) {
			// printf("Computer %d marked!\n", compFrom);
			res.elapsedTime += computers[compFrom].poodleTime;
		}
		res.elapsedTime += currentConn->transmissionTime;
		// printf("CURRENTLY CALCULATED TIME: %d\n", res.elapsedTime);

		finalComp = compTo;
	}

	res.elapsedTime += computers[finalComp].poodleTime;
	BookmarkFree(vists);
	// printf("ELAPSED TIME: %d\n", res.elapsedTime);

	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 2

struct chooseSourceResult chooseSource(struct computer computers[],
                                       int numComputers,
                                       struct connection connections[],
                                       int numConnections) {
	struct chooseSourceResult res = {0, 0, NULL};
	DiGraph digraph = DiGraphFromNetwork(numComputers, numConnections,
	                                     computers, connections);

	for (int i = 0; i < digraph->vertices; i++) {
		struct nodeReachability *r = DiGraphNodeReachability(digraph, i);
		// DEBUG CODE:
		// printf("Computers accessible from computer %d: ", r->nodeSrc);
		// for (int comp = 0; comp < r->accessCount; comp++) {
		// 	printf("%d ", r->nodeAccessible[comp]);
		// }
		// printf("\n");
		if (r->accessCount > res.numComputers) {
			res.numComputers = r->accessCount;
			res.sourceComputer = r->nodeSrc;
			if (res.computers != NULL) {
				int *memDump = res.computers;
				res.computers = NULL;
				free(memDump);
			}
			res.computers = copyArrayInt(r->nodeAccessible, r->accessCount);
		}
		free(r->nodeAccessible);
		free(r);
	}
	DiGraphFree(digraph);

	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 3

struct poodleResult poodle(struct computer computers[], int numComputers,
                           struct connection connections[], int numConnections,
                           int sourceComputer) {
	struct poodleResult res = {0, NULL};

	Fullgraph graph =
	    FullgraphGenerate(numComputers, numConnections, computers, connections);
	struct djikstraData *travData = FullgraphDjikstra(graph, sourceComputer);
	res.numSteps = travData->visits;
	res.steps = processDjikstraData(travData);

	// free data
	free(travData->distance);
	free(travData->predecessor);
	free(travData);
	FullgraphFree(graph);
	return res;
}

////////////////////////////////////////////////////////////////////////
// Task 4

/**
 * Describe your solution in detail here:
 *
 * TODO
 */
struct poodleResult advancedPoodle(struct computer computers[],
                                   int numComputers,
                                   struct connection connections[],
                                   int numConnections, int sourceComputer) {
	struct poodleResult res = {0, NULL};
	return res;
}

////////////////////////////////////////////////////////////////////////
// Local functions.

static int *copyArrayInt(int *arr, int arrSize) {
	int *newArr = malloc(sizeof(*newArr) * arrSize);
	for (int ix = 0; ix < arrSize; ix++) {
		newArr[ix] = arr[ix];
	}
	return newArr;
}

static struct step *processDjikstraData(struct djikstraData *data) {
	struct step *steps = malloc(sizeof(*steps) * data->visits);
	PqNeighbours pq = PqNeighboursNew();

	for (int vertexIx = 0; vertexIx < data->vertices; vertexIx++) {
		if (data->distance[vertexIx] == INFINITY) {
			// vertexIx--;
			continue;
		}
		PqNeighboursPush(pq, vertexIx, data->distance[vertexIx]);
	}

	// assigning computers and poodle time
	for (int step = 0; step < data->visits; step++) {
		if (pq->count == 0) {
			printf("Warning: poodle.c 197, priority queue ran out before step "
			       "reached visited.\n");
			break;
		}
		struct neighbourPath newStep = PqNeighboursPop(pq);
		steps[step].time = newStep.cost;
		steps[step].computer = newStep.vertexTo;
		steps[step].recipients = NULL;
	}

	// assigning recipients for each computers
	for (int step = 0; step < data->visits; step++) {
		int predecessor = steps[step].computer;
		for (int vertex = 0; vertex < data->vertices; vertex++) {
			if (data->predecessor[vertex] == predecessor) {
				steps[step].recipients =
				    addComputer(steps[step].recipients, vertex);
			}
		}
	}

	PqNeighboursFree(pq);
	return steps;
}

static struct computerList *addComputer(struct computerList *list,
                                        int computer) {
	// base case
	if (list == NULL) {
		struct computerList *newNode = malloc(sizeof(*newNode));
		newNode->next = NULL;
		newNode->computer = computer;
		return newNode;
	}

	// recursive case
	list->next = addComputer(list->next, computer);
	return list;
}
