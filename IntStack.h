/* Stack ADT for integers
 */
#ifndef IntStack_h_INCLUDED
#define IntStack_h_INCLUDED

struct stackNode {
	struct stackNode *next;
	int num;
};

typedef struct {
	struct stackNode *head;
	int count;
} *IntStack;

/* Create new IntStack.
 * Structure is allocated on heap memory
 * and must be manually freed.
 */
IntStack IntStackNew();

/* Push integer into IntStack
 */
void IntStackPush(IntStack stack, int num);

/* Pop integer into IntStack
 */
int IntStackPop(IntStack stack);

/* Free IntStack from memory
 */
void IntStackFree(IntStack stack);

#endif  // IntStack_h_INCLUDED
