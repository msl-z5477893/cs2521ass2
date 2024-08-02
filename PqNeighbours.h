/* A helper priority queue ADT for Fullgraph.
 * Given a node, the queue contains information
 * of all its neighbours.
 * sorts each neighbour according to the cost it takes to
 * access each one accordingly.
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

/* Create new priority queue.
 */
PqNeighbours PqNeighboursNew();

/* free priority queue from memory
 */
void PqNeighboursFree(PqNeighbours pq);

/* push neighbouring node information to queue
 */
void PqNeighboursPush(PqNeighbours pq, int vertexTo, int cost);

/* pop from priority queue i.e. the neighbour closest to node in queue
 */
struct neighbourPath PqNeighboursPop(PqNeighbours pq);

#endif  // PqNeighbours_h_INCLUDED
