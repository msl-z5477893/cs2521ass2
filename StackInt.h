/* Stack data structure for integers.
 */
#ifndef StackInt_h_INCLUDED
#define StackInt_h_INCLUDED

struct stackNodeInt {
	int num;
	struct stackNodeInt *next;
};

typedef struct {
	int count;
	struct stackNodeInt *head;
} *StackInt;

StackInt StackIntNew();

void StackIntFree(StackInt stack);

void StackIntPush(StackInt stack, int num);

int StackIntPop(StackInt stack);

#endif  // StackInt_h_INCLUDED
