// Fullgraph implementation goes here.

#include "Fullgraph.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Bookmark.h"
#include "PqNeighbours.h"
#include "StackInt.h"

#define INF 2147483647

struct weightedDestEdge {
	int dest;
	int edgeCost;
};

struct weightedDestEdge *getEdgesOrdered(int *edgeArray, int arrCount,
                                         int *newArrCountOut);
static bool directionalAccess(int securityFrom, int securityTo);

Fullgraph FullgraphGenerate(int numComps, int numConns, struct computer comps[],
                            struct connection conns[]) {
	// memory allocations and default values
	Fullgraph graph = malloc(sizeof(*graph));
	graph->vertices = numComps;
	graph->vertexCost = malloc(sizeof(*graph->vertexCost) * graph->vertices);
	graph->edges = malloc(sizeof(*graph->edges) * graph->vertices);
	for (int vertexIx = 0; vertexIx < graph->vertices; vertexIx++) {
		graph->edges[vertexIx] =
		    malloc(sizeof(*graph->edges[vertexIx]) * graph->vertices);

		for (int vdestIx = 0; vdestIx < graph->vertices; vdestIx++) {
			graph->edges[vertexIx][vdestIx] = 0;
		}
	}
	graph->edgeCount = malloc(sizeof(*graph->edgeCount) * graph->vertices);
	for (int vertexIx = 0; vertexIx < graph->vertices; vertexIx++) {
		graph->edgeCount[vertexIx] = 0;
	}

	// assignment
	for (int compIx = 0; compIx < graph->vertices; compIx++) {
		graph->vertexCost[compIx] = comps[compIx].poodleTime;
	}

	for (int conn = 0; conn < numConns; conn++) {
		int idCompA = conns[conn].computerA;
		int idCompB = conns[conn].computerB;
		int connCost = conns[conn].transmissionTime;
		int secA = comps[idCompA].securityLevel;
		int secB = comps[idCompB].securityLevel;
		if (directionalAccess(secA, secB)) {
			graph->edges[idCompA][idCompB] = connCost;
			graph->edgeCount[idCompA]++;
		}
		if (directionalAccess(secB, secA)) {
			graph->edges[idCompB][idCompA] = connCost;
			graph->edgeCount[idCompB]++;
		}
	}

	// return
	return graph;
}

void FullgraphFree(Fullgraph graph) {
	for (int vtx = 0; vtx < graph->vertices; vtx++) {
		free(graph->edges[vtx]);
	}
	free(graph->edgeCount);
	free(graph->edges);
	free(graph->vertexCost);
	free(graph);
}

// TODO: incomplete
struct djikstraData *FullgraphDjikstra(Fullgraph g, int vertexSrc) {
	struct djikstraData *data = malloc(sizeof(*data));
	// bool *visited = malloc(sizeof(*visited) * g->vertices);
	data->vertices = g->vertices;
	data->visits = 0;
	data->distance = malloc(sizeof(*data->distance) * data->vertices);
	data->predecessor = malloc(sizeof(*data->distance) * data->vertices);
	for (int ix = 0; ix < data->vertices; ix++) {
		data->predecessor[ix] = -1;
		data->distance[ix] = INF;
	}
	data->distance[vertexSrc] = 0;

	Bookmark visited = BookmarkInit(data->vertices);
	PqNeighbours pqueue = PqNeighboursNew();
	// StackInt stack = StackIntNew();

	// StackIntPush(stack, vertexSrc);
	PqNeighboursPush(pqueue, vertexSrc, data->distance[vertexSrc]);
	while (pqueue->count > 0) {
		// int currVtx = PqNeighbours(stack);
		struct neighbourPath currNode = PqNeighboursPop(pqueue);
		int currVtx = currNode.vertexTo;
		int currDist = currNode.cost;
		BookmarkMark(visited, currVtx);
		int edgesLiveNum = 0;
		struct weightedDestEdge *liveEdges =
		    getEdgesOrdered(g->edges[currVtx], g->vertices, &edgesLiveNum);
		/* FIXME: too much nesting
                 * this error may cause major marking penalties
                 */
		for (int edgesIx = 0; edgesIx < g->edgeCount[currVtx];) {
			if (g->edges[currVtx][edgesIx] > 0) {
				int newDist = g->edges[currVtx][edgesIx] + currDist;
				if (newDist < data->distance[edgesIx]) {
					data->distance[edgesIx] = newDist;
					data->predecessor[edgesIx] = currVtx;
					PqNeighboursPush(pqueue, edgesIx, newDist);
				}
			}
		}
	}

	printf("WARNING: used function FullgraphDjikstra() in Fullgraph.c is "
	       "potentially buggy.\n");

	// TODO: write debugging code here

	PqNeighboursFree(pqueue);
	BookmarkFree(visited);
	return data;
}

static bool directionalAccess(int securityFrom, int securityTo) {
	return !(securityTo > securityFrom + 1);
}

struct weightedDestEdge *getEdgesOrdered(int *edgeArray, int arrCount,
                                         int *newArrCountOut) {
	PqNeighbours pq = PqNeighboursNew();
	*newArrCountOut = 0;
	for (int ix = 0; ix < arrCount; ix++) {
		if (edgeArray[ix] != 0) {
			PqNeighboursPush(pq, ix, edgeArray[ix]);
			(*newArrCountOut)++;
		}
	}
	struct weightedDestEdge *edges = malloc(sizeof(*edges) * (*newArrCountOut));
	for (int ix = 0; ix < *newArrCountOut; ix++) {
		struct neighbourPath path = PqNeighboursPop(pq);
		edges[ix].dest = path.vertexTo;
		edges[ix].edgeCost = path.cost;
	}
	PqNeighboursFree(pq);
	return edges;
}
