#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this item
	int TaskName;  // task name 
	int Etime; //execution time
	int Rtime; // release time
	int Dline; // deadline
	struct HeapNode *parent; //pointer to parent
	struct HeapNode *left; //pointer to left child
	struct HeapNode *right; //pointer to right child
} HeapNode;

//data type for a priority queue (heap) 
typedef struct Heap{ //this is heap header
	int  size;      // count of items in the heap
	HeapNode *LastNode; // last node pointer 
	HeapNode *root; // pointer to the root of heap
} Heap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d, HeapNode *L, HeapNode *R, HeapNode *P)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // L, R, P: pointers to left child, right child and parent, respectively 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;
	new->left = L; // left child
	new->right = R; // righ child
	new->parent = P; // parent
	return new;
}

// create a new empty heap-based priority queue
 Heap *newHeap()
{ // this function creates an empty heap-based priority queue
	Heap *T;
	T = malloc(sizeof(Heap));
	assert (T != NULL);
	T->size = 0;
	T->LastNode=NULL; 
	T->root = NULL;
	return T;
}

// put the time complexity analysis for Insert() here    
void Insert(Heap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this item already exists in T 	 
  //put your code here
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(Heap *T)
{
 // put your code here
}

int Min(Heap *T)
{
  // put your code here
}


// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m )
{
 // put your code here
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}
