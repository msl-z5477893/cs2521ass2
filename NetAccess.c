#include "NetAccess.h"

#include "poodle.h"

NetAccess NetAccessNew() {
	NetAccess na = {.count = 0, .head = NULL, .tail = NULL};
	return na;
}

void NetAccessDeinit(NetAccess na) {
	struct node *curr = na.head;
	while (curr != NULL) {
		struct node *deleteThis = curr;
		curr = curr->next;
		free(deleteThis);
	}
}

void NetAccessAdd(NetAccess na, struct computer *comp) {
	struct node *item = malloc(sizeof(*item));
	item->comp = comp;
	item->next = NULL;

	if (na.head == NULL) {
		na.head = item;
		na.tail = item;
	} else {
		na.tail->next = item;
		na.tail = na.tail->next;
	}

	na.count++;
}

struct computer *NetAccessPop(NetAccess na) {
	struct node *pop = na.head;
	struct computer *comp = pop->comp;
	na.head = na.head->next;
	free(pop);
	return comp;
}

// TODO: implementation
int NetAccessInfluence(NetAccess na, int n);
