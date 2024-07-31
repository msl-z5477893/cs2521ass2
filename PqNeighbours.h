/* A helper priority queue ADT for Fullgraph.
 * Given a node, the queue contains information
 * of all its neighbours.
 * sorts all edge data based on cost.
 */

#ifndef PqNeighbours_h_INCLUDED
#define PqNeighbours_h_INCLUDED

struct neighbourPath {
	int vertexTo;
	int cost;
	struct neighbourPath *next;
};

typedef struct {
	int count;
	struct neighbourPath *head;
	struct neighbourPath *tail;
} *PqNeighbours;

PqNeighbours PqNeighboursNew();

void PqNeighboursFree(PqNeighbours pq);

void PqNeighboursPush(PqNeighbours pq, int vertexTo, int cost);

struct neighbourPath PqNeighboursPop(PqNeighbours pq);

#endif  // PqNeighbours_h_INCLUDED
