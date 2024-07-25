/*
 * IntSet is a data structure with the following properties
 * - has a dynamic size.
 * - fast insertion and searching.
 * - each element must be unique
 */
#ifndef IntSet_h_INCLUDED
#define IntSet_h_INCLUDED

#include <stdbool.h>
#define INTSET_CAPACITY 2

typedef struct {
	int *array;
	int capacity;
	int count;
} *IntSet;

/* Create new IntSet
 */
IntSet IntSetNew();

/* Free IntSet in memory
 */
void IntSetFree(IntSet list);

/* Add item to IntSet
 */
void IntSetAdd(IntSet list, int num);

/* Check if an item exists in IntSet
 * Uses binary search algorithm
 */
bool IntSetHasItem(IntSet list, int num);

#endif  // IntSet_h_INCLUDED
