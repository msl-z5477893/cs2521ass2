// Fullgraph implementation goes here.

#include "Fullgraph.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 2147483647

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
		}
		if (directionalAccess(secB, secA)) {
			graph->edges[idCompB][idCompA] = connCost;
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
	data->vertices = g->vertices;
	data->visits = 0;
	data->distance = malloc(sizeof(*data->distance) * data->vertices);
	data->predecessor = malloc(sizeof(*data->distance) * data->vertices);
	for (int ix = 0; ix < data->vertices; ix++) {
		data->predecessor[ix] = -1;
		data->distance[ix] = INF;
	}

	data->distance[vertexSrc] = 0;

	printf("WARNING: used function FullgraphDjikstra() in Fullgraph.c is "
	       "incomplete.\n");
	return data;
}

static bool directionalAccess(int securityFrom, int securityTo) {
	return !(securityTo > securityFrom + 1);
}
