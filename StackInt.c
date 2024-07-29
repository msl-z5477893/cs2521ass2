#include "StackInt.h"
#include <stdlib.h>

static struct stackNodeInt *newNode(int num);

StackInt StackIntNew() {
	StackInt stack = malloc(sizeof(*stack));
	stack->count = 0;
	stack->head = NULL;
	return stack;
}

void StackIntPush(StackInt stack, int num) {
	struct stackNodeInt *new = newNode(num);
	// if (stack->count == 0) {
	// 	new->next = stack->head;
	// }
	new->next = stack->head;
	stack->head = new;
	stack->count++;
}

int StackIntPop(StackInt stack) {
	struct stackNodeInt *pop = stack->head;
	int result = pop->num;
	stack->head = stack->head->next;
	free(pop);
	stack->count--;
	return result;
}

void StackIntFree(StackInt stack) {
	while (stack->count != 0) {
		StackIntPop(stack);
	}
	free(stack);
}

// LOCAL FUNCTIONS

static struct stackNodeInt *newNode(int num) {
	struct stackNodeInt *n = malloc(sizeof(*n));
	n->next = NULL;
	n->num = num;
	return n;
}
