/*
 * NetAccess.h
 *
 * A queue structure for accessing between computers in the network.
 * This implementation can be done on the stack.
 */

#ifndef NetAccess_h_INCLUDED
#define NetAccess_h_INCLUDED

#include <stdlib.h>

#include "Network.h"
#include "poodle.h"

struct node {
	struct node *next;
	struct computer *comp;
};

struct netAccess {
	struct node *head;
	struct node *tail;
	size_t count;
};

typedef struct netAccess NetAccess;

/*
 * Initialise a new queue.
 */
NetAccess NetAccessInit(void);

/*
 * Include computer for checking.
 */
void NetAccessAdd(NetAccess na, struct computer *comp);

/*
 * Removes item from queue
 */
struct computer *NetAccessPop(NetAccess na);

/*
 * Deinitialise NetAccess structure, removed any data within this structure
 * that is stored on the heap.
 */
void NetAccessDeinit(NetAccess na);

/*
 * Given a computer and details of a network,
 * give the number of computers the provided computer
 * can access.
 */
int NetAccessInfluence(NetAccess na, int n, struct network netData);

#endif
