/* IntPq is a priority queue for integers.
 * This has the following properties
 * - all integers are inserted into the queue in ascending order.
 */

#ifndef IntPq_h_INCLUDED
#define IntPq_h_INCLUDED

struct pqNode {
	struct pqNode *next;
	int num;
};

typedef struct {
	struct pqNode *head;
	struct pqNode *tail;
	int count;
} *IntPq;

/* Create new IntPq.
 * Performance: O(1)
 */
IntPq IntPqNew();

/* Free an IntPq.
 * Performance: O(n)
 */
void IntPqFree(IntPq pq);

/* Insert integer into IntPq.
 * Performance: O(n)
 */
void IntPqPush(IntPq pq, int num);

/* Remove integer in IntPq.
 * Performance: O(1)
 */
int IntPqPop(IntPq pq);

/* Show contents of Pq.
 * For debugging purposes.
 * Performance: O(n)
 */
void IntPqDebugPrint(IntPq pq);

#endif  // IntPq_h_INCLUDED
