#include <stdlib.h>

#include "PqNeighbours.h"

static struct neighbourPath *newNode(int vertexTo, int cost);

PqNeighbours PqNeighboursNew() {
	PqNeighbours pq = malloc(sizeof(*pq));
	pq->count = 0;
	pq->head = NULL;
	pq->tail = NULL;
	return pq;
}

void PqNeighboursFree(PqNeighbours pq) {
	while (pq->count != 0) {
		PqNeighboursPop(pq);
	}
	free(pq);
}

void PqNeighboursPush(PqNeighbours pq, int vertexTo, int cost) {
	struct neighbourPath *node = newNode(vertexTo, cost);
	// cases given min and max cost of edges in queue (in order)
	// - new edge lower than min
	// - new edge higher/eql than max
	// - somewhere in between
	if (pq->count == 0) {
		pq->head = node;
		pq->tail = node;
	} else if (node->cost < pq->head->cost) {
		node->next = pq->head;
		pq->head = node;
	} else if (node->cost >= pq->tail->cost) {
		pq->tail->next = node;
		pq->tail = pq->tail->next;
	} else {
		// since we considered head + tail in previous one, here we
		// consider the body.
		struct neighbourPath *listAnchor = pq->head;
		while (listAnchor->next->cost < node->cost) {
			listAnchor = listAnchor->next;
		}
		node->next = listAnchor->next;
		listAnchor->next = node;
	}
	pq->count++;
}

struct neighbourPath PqNeighboursPop(PqNeighbours pq) {
	struct neighbourPath *pop = pq->head;
	struct neighbourPath res = {
	    .cost = pop->cost, .next = NULL, .vertexTo = pop->vertexTo};
	free(pop);
	pq->count--;
	return res;
}

static struct neighbourPath *newNode(int vertexTo, int cost) {
	struct neighbourPath *n = malloc(sizeof(*n));
	n->cost = cost;
	n->vertexTo = vertexTo;
	n->next = NULL;
	return n;
}
