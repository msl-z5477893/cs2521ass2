// IntStack implementation goes here

#include "IntStack.h"
#include <stdlib.h>

static struct stackNode *newNode(int num);

IntStack IntStackNew() {
	IntStack stack = malloc(sizeof(*stack));
	stack->head = NULL;
	stack->count = 0;
	return stack;
}

void IntStackPush(IntStack stack, int num) {
	struct stackNode *node = newNode(num);
	if (stack->head == NULL) {
		stack->head = node;
	} else {
		node->next = stack->head;
		stack->head = node;
	}
	stack->count++;
}

int IntStackPop(IntStack stack) {
	struct stackNode *pop = stack->head;
	int res = pop->num;
	stack->head = stack->head->next;
	free(pop);
	stack->count--;
	return res;
}

void IntStackFree(IntStack stack) {
	while (stack->count != 0) {
		IntStackPop(stack);
	}
	free(stack);
}

/* Create a new node for IntStack
 */
static struct stackNode *newNode(int num) {
	struct stackNode *node = malloc(sizeof(*node));
	node->num = num;
	node->next = NULL;
	return node;
}

