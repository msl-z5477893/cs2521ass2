#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "AVLTreeInt.h"

// LOCAL STRUCTS
/* a node for linked list implementations
 */
struct llNodePtr {
	struct llNodePtr *next;
	struct intTreeNode *tree;
};

/* a queue structure
 */
struct queue {
	int count;
	struct llNodePtr *head;
	struct llNodePtr *tail;
};

/* a wrapper struct for an integer array,
 * used for extra functionality
 */
struct arrayInt {
	int capacity;
	int cursor;
	int *array;
};

// LOCAL FUNCTION HEADERS

// misc function headers

/* create new node for tree
 */
static struct intTreeNode *newNode(int num);

/* insert a new node as the left branch / right branch of
 * existing node, returning the height of the tree.
 * Is called recursively until a space is found.
 */
static int insertFromRoot(struct intTreeNode *root, struct intTreeNode *node);

/* get the difference between lesser and greater subtrees.
 */
static int balance(struct intTreeNode *root);

/* inOrder traversal of a tree
 */
static void inorderAVL(struct intTreeNode *node, struct arrayInt *arr);

// arrayInt method headers

/* create new arrayInt structure
 */
static struct arrayInt *arrayIntNew(int size);

/* insert integer into arrayInt,
 * returns false if array is full.
 */
static bool arrayIntInsert(struct arrayInt *arr, int num);

/* copy internal array from arrayInt structure
 */
static int *arrayIntCopy(struct arrayInt *arr);

/* free array int buffer
 */
static void arrayIntFree(struct arrayInt *arr);

// queue method headers

static struct queue *queueInit();

static void queueDeinit(struct queue *q);

static void queuePush(struct queue *q, struct intTreeNode *ptr);

static struct intTreeNode *queuePop(struct queue *q);

// EXPORTED FUNCTIONS IMPLEMENTATIONS

AVLTreeInt AVLTreeIntNew() {
	AVLTreeInt tree = malloc(sizeof(*tree));
	tree->root = NULL;
	tree->count = 0;
	return tree;
}

void AVLTreeIntFree(AVLTreeInt tree) {
	if (tree->root != NULL) {
		printf("(AVLTreeIntFree()) AVLTreeInt.c line 94: ");
		printf("Final height: %d, balance %d\n", tree->root->height,
		       balance(tree->root));
	}
	struct queue *q = queueInit();
	queuePush(q, tree->root);
	while (q->count != 0) {
		struct intTreeNode *curr = queuePop(q);
		if (curr == NULL) {
			printf("UNEXPECTED BEHAVIOUR: null pointer in queue\n");
			printf("Message emitted from line 99.\n");
			continue;
		}
		if (curr->lesser != NULL) {
			queuePush(q, curr->lesser);
		}
		if (curr->greater != NULL) {
			queuePush(q, curr->greater);
		}
		free(curr);
	}
	queueDeinit(q);
	free(tree);
}

bool AVLTreeIntInsert(AVLTreeInt tree, int num) {
	struct intTreeNode *node = newNode(num);
	if (tree->root == NULL) {
		tree->root = node;
		tree->count++;
		return true;
	} else if (tree->root->num == node->num) {
		free(node);
		return false;
	}

	// printf("AVLTreeInt.c line 26: root node found, using recursive "
	//        "placement...\n");
	int newHeight = insertFromRoot(tree->root, node);
	if (newHeight == -1) {
		free(node);
		return false;
	} else {
		tree->root->height = newHeight;
		tree->count++;
		return true;
	}
}

int *AVLTreeIntFlatten(AVLTreeInt tree) {
	struct arrayInt *arr = arrayIntNew(tree->count);
	inorderAVL(tree->root, arr);
	int *results = arrayIntCopy(arr);

	// DEBUG CODE
	printf("(AVLTreeIntFlatten()) AVLTreeInt.c line 145: ");
	printf("Returned list: [");
	for (int ix = 0; ix < arr->capacity; ix++) {
		printf(" %d%c", results[ix], (ix + 1 < arr->capacity) ? ',' : ']');
	}
	printf("\n");
	arrayIntFree(arr);
	return results;
}

// LOCAL FUNCTIONS

// misc helper functions

static struct intTreeNode *newNode(int num) {
	struct intTreeNode *node = malloc(sizeof(*node));
	node->height = 0;
	node->num = num;
	node->greater = NULL;
	node->lesser = NULL;
	return node;
}

static int insertFromRoot(struct intTreeNode *root, struct intTreeNode *node) {
	if (node->num < root->num) {
		// if we found a free space for our new node
		// insert it there as a leaf and return 1
		if (root->lesser == NULL) {
			root->lesser = node;
			return 1;
		}

		// recursive call
		int foundHeight = insertFromRoot(root->lesser, node);
		if (foundHeight == -1) {
			return -1;
		} else {
			// we set the height of the subtree we called from
			// the function with the returned value then insert
			// it
			root->lesser->height = foundHeight;
			return foundHeight + 1;
		}
	} else if (node->num > root->num) {
		if (root->greater == NULL) {
			root->greater = node;
			return 1;
		}
		int foundHeight = insertFromRoot(root->greater, node);
		if (foundHeight == -1) {
			return -1;
		} else {
			root->greater->height = foundHeight;
			return foundHeight + 1;
		}
	}
	return -1;
}

static int balance(struct intTreeNode *root) {
	if (root->lesser == NULL && root->greater == NULL) {
		return 0;
	} else if (root->lesser == NULL || root->greater == NULL) {
		return root->height;
	}
	return abs(root->lesser->height - root->greater->height);
}

static void inorderAVL(struct intTreeNode *node, struct arrayInt *arr) {
	if (node == NULL) {
		return;
	}
	inorderAVL(node->lesser, arr);
	arrayIntInsert(arr, node->num);
	inorderAVL(node->greater, arr);
}

// arrayInt methods

static struct arrayInt *arrayIntNew(int size) {
	struct arrayInt *arr = malloc(sizeof(*arr));
	arr->capacity = size;
	arr->cursor = 0;
	arr->array = malloc(sizeof(*arr->array) * arr->capacity);
	return arr;
}

static void arrayIntFree(struct arrayInt *arr) {
	free(arr->array);
	free(arr);
}

static bool arrayIntInsert(struct arrayInt *arr, int num) {
	if (arr->cursor == arr->capacity) {
		return false;
	}
	arr->array[arr->cursor] = num;
	arr->cursor++;
	return true;
}

static int *arrayIntCopy(struct arrayInt *arr) {
	int *out = malloc(sizeof(*out) * arr->capacity);
	for (int ix = 0; ix < arr->capacity; ix++) {
		out[ix] = arr->array[ix];
	}
	return out;
}

// queue methods

static struct queue *queueInit() {
	struct queue *q = malloc(sizeof(*q));
	q->count = 0;
	q->head = NULL;
	q->tail = NULL;
	return q;
}

static void queueDeinit(struct queue *q) {
	// queue MUST be empty before freeing
	// otherwise something has gone wrong
	assert(q->count == 0);
	free(q);
}

static void queuePush(struct queue *q, struct intTreeNode *ptr) {
	struct llNodePtr *ptrNode = malloc(sizeof(*ptrNode));
	ptrNode->tree = ptr;
	ptrNode->next = NULL;

	if (q->count == 0) {
		q->head = ptrNode;
		q->tail = ptrNode;
	} else {
		q->tail->next = ptrNode;
		q->tail = q->tail->next;
	}

	q->count++;
}

static struct intTreeNode *queuePop(struct queue *q) {
	struct llNodePtr *pop = q->head;
	struct intTreeNode *treePtr = pop->tree;
	q->head = q->head->next;
	q->count--;
	free(pop);
	return treePtr;
}
