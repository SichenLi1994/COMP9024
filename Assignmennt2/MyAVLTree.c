#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define MAX(a, b)    ( (a) > (b) ? (a) : (b) )
#define HEIGHT(p)    ( (p==NULL) ? 0 : (((AVLTreeNode *)(p))->height) )

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a New AVLTreeNode
AVLTreeNode *NewAVLTreeNode(int k, int v )
{
	AVLTreeNode *New;
	New = (AVLTreeNode*)malloc(sizeof(AVLTreeNode));
	assert(New != NULL);
	New->key = k;
	New->value = v;
	New->height = 0; // height of this New node is set to 0
	New->left = NULL; // this node has no child
	New->right = NULL;
	New->parent = NULL; // no parent
	return New;
}

// create a New empty avl tree
AVLTree *NewAVLTree()
{
	AVLTree *T;
	T = (AVLTree*)malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}


static AVLTreeNode* LLrotation(AVLTreeNode* k2)
{
    AVLTreeNode* k1;
    k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = MAX( HEIGHT(k2->left), HEIGHT(k2->right)) + 1;
    k1->height = MAX( HEIGHT(k1->left), k2->height) + 1;
    return k1;
}


static AVLTreeNode* RRrotation(AVLTreeNode* k1)
{
    AVLTreeNode* k2;

    k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;

    k1->height = MAX( HEIGHT(k1->left), HEIGHT(k1->right)) + 1;
    k2->height = MAX( HEIGHT(k2->right), k1->height) + 1;
    return k2;
}


static AVLTreeNode* LRrotation(AVLTreeNode* k3)
{
    k3->left = RRrotation(k3->left);
    return LLrotation(k3);
}


static AVLTreeNode* RLrotation(AVLTreeNode* k1)
{
    k1->right = LLrotation(k1->right);
    return RRrotation(k1);
}


// put your time complexity analysis of CreateAVLTree() here
/*AVLTree *CreateAVLTree(const char *filename)
{

  
}*/

AVLTreeNode* cloneTreeHelp(AVLTreeNode* curTree){
    if(curTree==NULL) return NULL;
    AVLTreeNode* left = cloneTreeHelp(curTree->left);
    AVLTreeNode* right = cloneTreeHelp(curTree->right);
    AVLTreeNode* NewNode = NewAVLTreeNode(curTree->key,curTree->value);
    NewNode->left = left;
    NewNode->right = right;
    return NewNode;

}

// put your time complexity analysis for CloneAVLTree() here
AVLTree *CloneAVLTree(AVLTree *T)
{
    AVLTree* NewT =  NewAVLTree();
    NewT->root =  cloneTreeHelp(T->root);
    NewT->size = T->size;
    return NewT;
}
 

AVLTreeNode* InsertHelp(AVLTreeNode* tree,int k,int v){
	if(tree==NULL){
		//New a node
		tree = NewAVLTreeNode(k,v);
		assert(tree!=NULL);
	}
	else if(k<tree->key){
		// insert into left
		tree->left = InsertHelp(tree->left,k,v);
		if(HEIGHT(tree->left) - HEIGHT(tree->right) == 2){
			if(k<tree->left->key)
				tree = LLrotation(tree);
			else
				tree = LRrotation(tree);
		}
	}
	else if(k>tree->key){
		tree->right = InsertHelp(tree->right,k,v);
		if(HEIGHT(tree->right) - HEIGHT(tree->left) == 2){
			if(k>tree->right->key)
				tree = RRrotation(tree);
			else
				tree = RLrotation(tree);
		}
	}
	tree->height = MAX(HEIGHT(tree->left),HEIGHT(tree->right)) + 1;
	return tree;
}

// O(logN)    
int InsertNode(AVLTree *T, int k, int v)
{
	AVLTreeNode* root = InsertHelp(T->root,k,v);
	if(root==NULL) return 0;
	T->root = root;
	T->size += 1;
	return 1;
}

AVLTreeNode* AVLTreeMaximum(AVLTreeNode* tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->right != NULL)
        tree = tree->right;
    return tree;
}
AVLTreeNode* AVLTreeMinimum(AVLTreeNode* tree)
{
    if (tree == NULL)
        return NULL;

    while(tree->left != NULL)
        tree = tree->left;
    return tree;
}

static AVLTreeNode* DeleteHelp(AVLTreeNode* tree,int k){
    if (tree==NULL)
        return NULL;

    if (k < tree->key)        // 待删除的节点在"tree的左子树"中
    {
        tree->left = DeleteHelp(tree->left, k);
        if (HEIGHT(tree->right) - HEIGHT(tree->left) == 2)
        {
            AVLTreeNode *r =  tree->right;
            if (HEIGHT(r->left) > HEIGHT(r->right))
                tree = RLrotation(tree);
            else
                tree = RRrotation(tree);
        }
    }
    else if (k > tree->key)// 待删除的节点在"tree的右子树"中
    {
        tree->right = DeleteHelp(tree->right, k);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(tree->left) - HEIGHT(tree->right) == 2)
        {
            AVLTreeNode *l =  tree->left;
            if (HEIGHT(l->right) > HEIGHT(l->left))
                tree = LRrotation(tree);
            else
                tree = LLrotation(tree);
        }
    }
    else    // tree是对应要删除的节点。
    {
        // tree的左右孩子都非空
        if ((tree->left) && (tree->right))
        {
            if (HEIGHT(tree->left) > HEIGHT(tree->right))
            {
                AVLTreeNode *max = AVLTreeMaximum(tree->left);
                tree->key = max->key;
                tree->left = DeleteHelp(tree->left, max->key);
            }
            else
            {
                AVLTreeNode *min = AVLTreeMinimum(tree->right);
                tree->key = min->key;
                tree->right = DeleteHelp(tree->right, min->key);
            }
        }
        else
        {
            AVLTreeNode *tmp = tree;
            tree = tree->left ? tree->left : tree->right;
            free(tmp);
        }
    }
    return tree;
}


// put your time complexity for DeleteNode() here
int DeleteNode(AVLTree *T, int k)
{
	DeleteHelp(T->root,k);
	T->size -= 1;
	return 0;
}



AVLTreeNode* SearchHelp(AVLTreeNode* tree,int k){
	if(tree==NULL) return NULL;
	if(k>tree->key) return SearchHelp(tree->right,k);
	else if(k<tree->key) return SearchHelp(tree->left,k);
	else return tree;
}

// put your time complexity analysis for Search() here
AVLTreeNode *Search(AVLTree *T, int k)
{
	return SearchHelp(T->root,k);
}


void FreeHelp(AVLTreeNode* tree){
	if(tree==NULL) return;
	FreeHelp(tree->left);
	DeleteHelp(tree,tree->key);
	FreeHelp(tree->right);
}
// put your time complexity analysis for freeAVLTree() here
void FreeAVLTree(AVLTree *T)
{
	FreeHelp(T->root);
	free(T);
}

void PrintHelp(AVLTreeNode* tree){
	if(tree==NULL) return;
	PrintHelp(tree->left);
	printf("(%d,%d)\n",tree->key,tree->value);
	PrintHelp(tree->right);
}

// put your time complexity analysis for PrintAVLTree() here
void PrintAVLTree(AVLTree *T)
{
	PrintHelp(T->root);
}

int main() //sample main for testing 
{ 
/*	int i,j;*/
/* AVLTree *tree1, *tree2, *tree3, *tree4;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items with distinct keys
 tree2=CreateAVLTree("file1.txt"); 
 PrintAVLTree(tree2);
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 
 tree4=NewAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  node1=Search(tree4,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
    j=DeleteNode(tree4, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 return 0; */
	AVLTree *tree = NewAVLTree();
	tree->root = NewAVLTreeNode(156,1);
	tree->size = 1;
  PrintAVLTree(tree);
	InsertNode(tree,5,2);
	InsertNode(tree,-30,3);
	InsertNode(tree,7,3);
	InsertNode(tree,1,3);
	InsertNode(tree,5630,3);
PrintAVLTree(tree);
	InsertNode(tree,0,3);
	InsertNode(tree,0,3);
	InsertNode(tree,0,3);
	InsertNode(tree,0,3);
	InsertNode(tree,0,3);
	DeleteNode(tree,0);
	InsertNode(tree,-30,3);
	//PrintAVLTree(tree);
}
