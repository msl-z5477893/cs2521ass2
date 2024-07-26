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

/* Add item to IntSet.
 * Returns true if inserted, false otherwise.
 */
bool IntSetAdd(IntSet list, int num);

/* Check if an item exists in IntSet
 * Uses binary searching, so set has to be ordered by default.
 */
bool IntSetQuickCheck(IntSet list, int num);

/* Print the contents of integer set.
 * For debugging purposes
 */
void IntSetDebugPrint(IntSet set);

#endif  // IntSet_h_INCLUDED
