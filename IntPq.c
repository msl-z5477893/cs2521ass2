// IntPq source file.
#include "IntPq.h"
#include <stdlib.h>

static struct pqNode *newNode(int num);

IntPq IntPqNew() {
	IntPq pq = malloc(sizeof(*pq));
	pq->head = NULL;
	pq->tail = NULL;
	pq->count = 0;
	return pq;
}

void IntPqPush(IntPq pq, int num) {
	struct pqNode *item = newNode(num);
	int max = pq->tail->num;
	int min = pq->head->num;
	if (pq->count == 0) {
		pq->head = item;
		pq->tail = item;
	} else if (num < min) {
		item->next = pq->head;
		pq->head = item;
	} else if (num > max) {
		pq->tail->next = item;
		pq->tail = pq->tail->next;
	} else {
		struct pqNode *curr = pq->head;
		while (num > curr->num) {
			curr = curr->next;
		}
		// swap numbers between new node and current
		int tempNum = curr->num;
		curr->num = num;
		item->num = tempNum;

		// swap the connecting addresses and connect
		// current node to new node.
		struct pqNode *tmpAddr = curr->next;
		curr->next = item;
		item->next = tmpAddr;
	}
	pq->count++;
}

int IntPqPop(IntPq pq) {
	struct pqNode *pop = pq->head;
	pq->head = pq->head->next;
	int num = pop->num;
	free(pop);
	pq->count--;
	return num;
}

void IntPqFree(IntPq pq) {
	while (pq->count != 0) {
		IntPqPop(pq);
	}
	free(pq);
}

/* Helper function for creating a new IntPq node.
 */
static struct pqNode *newNode(int num) {
	struct pqNode *node = malloc(sizeof(*node));
	node->num = num;
	node->next = NULL;
	return node;
}
