// set functions go here ig

#include <stdbool.h>
#include <stdlib.h>

#include "IntSet.h"
IntSet IntSetNew() {
	IntSet set = malloc(sizeof(*set));
	set->count = 0;
	set->capacity = INTSET_CAPACITY;
	set->array = malloc(sizeof(*set->array) * set->capacity);
	return set;
}

void IntSetFree(IntSet set) {
	free(set->array);
	free(set);
}

void IntSetAdd(IntSet set, int num) {
	int newCount = set->count + 1;
	if (newCount >= set->capacity) {
		set->capacity *= INTSET_CAPACITY;
		set->array = realloc(set->array, sizeof(*set->array) * set->capacity);
	}
	set->array[set->count] = num;
	set->count = newCount;
}

bool IntSetHasItem(IntSet set, int num) {
	int left = 0;
	int right = set->capacity - 1;
	while (left <= right) {
		// int mid = (left + right) / 2;
		// some code documentations use this,
		// why? idk
		int mid = left + (right - left) / 2;
		if (num < set->array[mid]) {
			right = mid - 1;
		} else if (num > set->array[mid]) {
			left = mid + 1;
		} else {
			return true;
		}
	}
	return false;
}
