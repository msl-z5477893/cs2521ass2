// DiGraph source goes here

#include "DiGraph.h"
#include <stdlib.h>

DiGraph DiGraphFromNetwork(int numComputers, int numConnections,
                           struct computer *computers,
                           struct connection *connections) {
	// return NULL;
	DiGraph dgraph = malloc(sizeof(*dgraph));
	dgraph->vertices = numComputers;
	dgraph->adjacency = malloc(sizeof(*dgraph->adjacency) * dgraph->vertices);
	// TODO: loop through network data and build adjacency list.
	return dgraph;
}

