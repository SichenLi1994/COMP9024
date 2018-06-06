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

void swapHeapNode(HeapNode *a, HeapNode *b){
	int tmp;
	tmp = a->TaskName; a->TaskName = b->TaskName; b->TaskName = tmp;
	tmp = a->Etime   ; a->Etime = b->Etime; b->Etime = tmp;
	tmp = a->Rtime; a->Rtime = b->Rtime;b->Rtime=tmp;
	tmp = a->Dline; a->Dline=b->Dline;b->Dline=tmp;
	tmp = a->key;a->key=b->key;b->key=tmp;
}


// find the new LastNode is like flip bits , at most O(log n)
// the loop using swapHeapNode is O(log n)
// as a whole: O(log n)
void Insert(Heap *T, int k, int n, int c, int r, int d)
{ 
	
	HeapNode *h = newHeapNode(k, n, c, r, d, 0, 0, 0);
	HeapNode *p;
	if(T->size == 0){
		T->root = T->LastNode = h;
		T->size = 1;
		return;
	}
	p=T->LastNode;
	if(p->parent && p == p->parent->left){
		p->parent->right = h;
		h->parent = p->parent;
		T->LastNode = h;
	}else{
		while(p->parent && p == p->parent->right){
			p=p->parent;
		}
		if(p->parent){
			p=p->parent->right;
		}
		while(p && p->left) p=p->left;
		p->left=h;
		h->parent=p;
		T->LastNode=h;
	}
	while(h->parent && h->key < h->parent->key){
		swapHeapNode(h, h->parent);
		h=h->parent;
	}
	T->size++;
	
}


// find the new LastNode is like flip bits , at most O(log n)
// the loop using swapHeapNode is O(log n)
// as a whole: O(log n)
HeapNode *RemoveMin(Heap *T)
{
   HeapNode *root, *p, *q;
   if(T->size == 1){
	   root = T->root;
	   T->size = 0;
	   T->root = T->LastNode = 0;
	   return root;
   }
   
   q = T->LastNode;
   p = T->root;
   swapHeapNode(p, q);
   root=T->LastNode;
   
   while(q->parent && q == q->parent->left){
	   q = q->parent;
   }

   if(q->parent){
	   q = q->parent->left;
   }
   
   while(q && q->right) q = q->right;
   T->LastNode = q;
   if(root == root->parent->left){
	   root->parent->left = 0;
   }else{
	   root->parent->right = 0;
   }
   
   T->size--;

   while(p->left || p->right){
	   if(p->left && p->right && p->key > p->left->key && p->key > p->right->key){
		   if(p->left->key < p->right->key){
			   swapHeapNode(p, p->left);
			   p = p->left;
		   }else{
			   swapHeapNode(p, p->right);
			   p = p->right;
		   }
	   }else if(p->left && p->key > p->left->key){
		    swapHeapNode(p, p->left);
			p = p->left;
	   }else if(p->right && p->key > p->right->key){
			swapHeapNode(p, p->right);
			p = p->right;
	   }else{
		   break;
	   }
   }
   
  
   
   return root;			
}

int Min(Heap *T)
{
  return T->root->key;
}

struct Allocate{
	int taskName;
	int core;
	int startTime;
};


int compareFunction(struct Allocate *a, struct Allocate *b){
	
	if(a->startTime != b->startTime)return a->startTime - b->startTime;
	if(a->taskName  != b->taskName )return a->taskName  - b->taskName;
	return 0;

}

// we read the data in O(n)
//   insert them in O(log n) * O(n) = O(n log n)
//   loop to calculate the best plan: O(n) * O(logn) * O(m)
// total: O(mnlogn)

int TaskScheduler(char *f1, char *f2, int m )
{
   Heap *heap = newHeap();
   FILE *fp = fopen(f1, "r");
   int  *start = (int *)malloc(sizeof(int) * m);
   if(!fp) {
	   printf("file1 does not exist\n");
	   return 0;
   }
   {
	   int taskname, cTime, rTime, dTime;
	   while(fscanf(fp, "%d%d%d%d", &taskname, &cTime, &rTime, &dTime) != EOF){
			if(cTime < 0 || rTime < 0 || dTime < 0){
				printf("input error when reading the attribute of the task %d\n", taskname);
				fclose(fp);
				while(heap->size > 0) free(RemoveMin(heap));
				free(start);
				free(heap);
				return 0;
			}
			Insert(heap,  dTime - cTime + rTime, taskname, cTime, rTime, dTime);
	   }
	   fclose(fp);
   }
   {
	   int n = heap->size;
	   struct Allocate *all = (struct Allocate *)malloc(sizeof(struct Allocate) * n);
	   for(int i = 0; i < m; i++){ start[i] = 0; }
	   int j = 0;
	   while(heap->size > 0){
		   HeapNode *h = RemoveMin(heap);
		   int minindex = -1, mintime = 9999999;
		   for(int i = 0; i < m; ++i){
			   if(mintime > start[i]) mintime = start[i], minindex = i;
		   }
		   if(mintime < h->Rtime) mintime = h->Rtime;
		   start[minindex] = mintime  + h->Etime;
		   
		   if(mintime+h->Etime > h->Dline){
			    free(h);
			    while(heap->size > 0) free(RemoveMin(heap));
				free(all);
				free(start);
				free(heap);
				return 0;
		   }
		   all[j].taskName = h->TaskName;
		   all[j].core = minindex + 1;
		   all[j].startTime = mintime;
		   j++;
		   free(h);
	   }
	   qsort(all, n, sizeof(struct Allocate), (int (*)(const void *, const void *))compareFunction);
	   {
	    FILE *fp = fopen(f2, "w");
	    for(int i=0; i<n; i++){
		     if(i > 0) fprintf(fp, " ");
		     fprintf(fp, "%d Core%d %d", all[i].taskName, all[i].core, all[i].startTime);
		}
		fclose(fp);
	   }
	   
	   free(all);
	   free(start);
	   free(heap);
	   return 1;
   }
}

int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("1No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("2No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("3No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("4No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("5No feasible schedule!\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("6No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}
