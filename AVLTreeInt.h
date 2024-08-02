/* A binary search tree for integers
 * used for faster insertion and lookup.
 *
 * NOTES: rebalancing method not created,
 * at this stage this tree is just a normal
 * binary search tree.
 */
#ifndef AVLTreeInt_h_INCLUDED
#define AVLTreeInt_h_INCLUDED

struct intTreeNode {
	struct intTreeNode *lesser;
	struct intTreeNode *greater;
	int height;
	int num;
};

typedef struct {
	struct intTreeNode *root;
	int count;
} *AVLTreeInt;

/* Create new AVL tree for integers
 */
AVLTreeInt AVLTreeIntNew();

/* Free AVL tree in memory
 */
void AVLTreeIntFree(AVLTreeInt tree);

/* Insert integer in tree.
 * If integer already in the tree, tree remains unchanged
 * and returns false, otherwise return true.
 */
bool AVLTreeIntInsert(AVLTreeInt tree, int num);

/* Flatten contents of tree into an integer array,
 * and stores this in heap memory.
 * Returned array stores this inorder.
 */
int *AVLTreeIntFlatten(AVLTreeInt tree);

#endif  // AVLTreeInt_h_INCLUDED
