// Fullgraph implementation goes here.

#include "Fullgraph.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Bookmark.h"
#include "PqNeighbours.h"

#define INF            2147483647
#define NO_PREDECESSOR -1

struct neighbourData {
	int dest;
	int edgeCost;
};

static struct neighbourData *findNeighbours(Fullgraph g, int vertexSrc,
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

// TODO: untested
struct djikstraData *FullgraphDjikstra(Fullgraph g, int vertexSrc) {
	struct djikstraData *data = malloc(sizeof(*data));
	data->vertices = g->vertices;
	data->distance = malloc(sizeof(*data->distance) * data->vertices);
	data->predecessor = malloc(sizeof(*data->distance) * data->vertices);
	data->visits = 0;
	for (int ix = 0; ix < data->vertices; ix++) {
		data->predecessor[ix] = -1;
		data->distance[ix] = INF;
	}
	//data->distance[vertexSrc] = 0;
	// account for source node cost
	data->distance[vertexSrc] = g->vertexCost[vertexSrc];

	Bookmark visited = BookmarkInit(data->vertices);
	PqNeighbours pqueue = PqNeighboursNew();

	PqNeighboursPush(pqueue, vertexSrc, data->distance[vertexSrc]);
	while (pqueue->count > 0) {
		struct neighbourPath currNode = PqNeighboursPop(pqueue);
		int currVtx = currNode.vertexTo;
		int currDist = currNode.cost;
		// NOTE: adding previous node cost when calculating the closest distance
		// to the next neighbour causes double counting.
		// int nodeCost = g->vertexCost[currVtx];
		// printf("Current node %d has cost %d which is %d units from source.\n",
		//        currVtx, nodeCost, currDist);
		BookmarkMark(visited, currVtx);

		int nodeCount = 0;
		struct neighbourData *neighbours =
		    findNeighbours(g, currVtx, &nodeCount);
		for (int nbIx = 0; nbIx < nodeCount; nbIx++) {
			int currentNeighbour = neighbours[nbIx].dest;
			// ignore predecessor
			if (currentNeighbour == data->predecessor[currVtx]) {
				continue;
			}
			// int edgeCost = neighbours[nbIx].edgeCost;
			int edgeCost = g->edges[currVtx][currentNeighbour];

			int nextNodeCost = g->vertexCost[currentNeighbour];
			// int newDist = edgeCost + currDist;
			int newDist = currDist + edgeCost + nextNodeCost;

			if (newDist < data->distance[currentNeighbour]) {
				// if (!BookmarkMark(visited, currVtx))
				// 	newDist += nodeCost;

				// printf("Calculate new distance between node %d to node %d: %d "
				//        "+ %d + %d = %d.\n",
				//        currVtx, currentNeighbour, currDist, edgeCost,
				//        nextNodeCost, newDist);
				// printf(
				//     "New recorded distance from source node to node %d: %d.\n",
				//     currentNeighbour, newDist);
				data->distance[currentNeighbour] = newDist;
				data->predecessor[currentNeighbour] = currVtx;
				PqNeighboursPush(pqueue, currentNeighbour,
				                 data->distance[currentNeighbour]);
			}
		}
		free(neighbours);
	}

	// account for vertexSrc, which will have -1 (no predecessor)
	data->visits++;
	// get num of visited nodes
	for (int vertex = 0; vertex < data->vertices; vertex++) {
		if (data->predecessor[vertex] != NO_PREDECESSOR) {
			data->visits++;
		}
	}

	// TODO: write debugging code here
	// printf("WARNING: used function FullgraphDjikstra() in Fullgraph.c is "
	//        "potentially buggy.\n");
	// printf("Results of Djikstra's algorithm: \n");
	// printf("From source vertex: %d\n", vertexSrc);
	// for (int ix = 0; ix < data->vertices; ix++) {
	// 	printf("Node %d -> (distance: %d, predecessor: %d) \n", ix,
	// 	       data->distance[ix], data->predecessor[ix]);
	// }

	PqNeighboursFree(pqueue);
	BookmarkFree(visited);
	return data;
}

static bool directionalAccess(int securityFrom, int securityTo) {
	return !(securityTo > securityFrom + 1);
}

static struct neighbourData *findNeighbours(Fullgraph graph, int vertexSrc,
                                            int *newArrCountOut) {
	PqNeighbours pq = PqNeighboursNew();
	*newArrCountOut = 0;
	for (int ix = 0; ix < graph->vertices; ix++) {
		if (graph->edges[vertexSrc][ix] != 0) {
			PqNeighboursPush(pq, ix, graph->edges[vertexSrc][ix]);
			(*newArrCountOut)++;
		}
	}
	struct neighbourData *resultPtr =
	    malloc(sizeof(struct neighbourData) * (*newArrCountOut));
	for (int ix = 0; ix < *newArrCountOut; ix++) {
		struct neighbourPath path = PqNeighboursPop(pq);
		resultPtr[ix].dest = path.vertexTo;
		resultPtr[ix].edgeCost = path.cost;
	}
	PqNeighboursFree(pq);
	return resultPtr;
}
