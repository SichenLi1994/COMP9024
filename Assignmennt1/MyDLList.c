#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data structures representing DLList

// data type for nodes
typedef struct DLListNode {
	int  value;  // value (int) of this list item
	struct DLListNode *prev;
	// pointer previous node in list
	struct DLListNode *next;
	// pointer to next node in list
} DLListNode;

//data type for doubly linked lists
typedef struct DLList{
	int  size;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it)
{
	DLListNode *new;
	new = malloc(sizeof(DLListNode));
	assert(new != NULL);
	new->value = it;
	new->prev = new->next = NULL;
	return new;
}

// create a new empty DLList
DLList *newDLList()
{
	struct DLList *L;

	L = malloc(sizeof (struct DLList));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// create a DLList from a text file
// O(n)
DLList *CreateDLListFromFileDlist(const char *filename)
{
	struct DLList* L = NULL;
	struct DLListNode* firstNode = NULL;
	struct DLListNode* curNode = NULL;
	int value;
	int count = 0;
	if(strcmp(filename,"stdin")==0){
		char buff[12];
		while(gets(buff)){
			if(buff[0]=='\0') break;
			count++;
			if(count==1){
				value = atoi(buff);
	    		firstNode = newDLListNode(value);
	    		curNode = firstNode;
	    	}
	    	else{
	    		value = atoi(buff);
	    		struct DLListNode* temp = newDLListNode(value);
	    		curNode->next = temp;
	    		temp->prev = curNode;
	    		curNode = temp;
	    	}
    	}
	}
	else{
	    FILE *fp=fopen(filename,"r");
	    assert(fp != NULL);
	    while(!feof(fp))
	    {
	    	count++;
	    	fscanf(fp,"%d ",&value);
	    	//printf("%d\n", value);
	    	if(count==1){
	    		firstNode = newDLListNode(value);
	    		curNode = firstNode;
	    	}
	    	else{
	    		struct DLListNode* temp = newDLListNode(value);
	    		curNode->next = temp;
	    		temp->prev = curNode;
	    		curNode = temp;
	    	}
	    }
	    fclose(fp);
	}
	L = newDLList();
    L->first = firstNode;
    L->last = curNode;
    L->size = count;
    return L;
}

// clone a DLList
// O(n)
DLList *cloneList(struct DLList *u)
{
	struct DLList* L = newDLList();
	struct DLListNode* uCurNode = u->first;
	struct DLListNode* firstNode = newDLListNode(uCurNode->value);
	struct DLListNode* LcurNode = firstNode;
	uCurNode = uCurNode->next;
	while(uCurNode!=NULL){
		struct DLListNode* temp = newDLListNode(uCurNode->value);
		LcurNode->next = temp;
		temp->prev = LcurNode;
		LcurNode = temp;
		uCurNode = uCurNode->next;
	}
	L->first = firstNode;
	L->last = LcurNode;
	L->size = u->size;
	return L;
}

// compute the union of two DLLists u and v
// O(n^2)
DLList *setUnion(struct DLList *u, struct DLList *v)
{
	struct DLListNode* uCurNode = u->first;
	DLList* result = cloneList(v);
	int size = v->size;
	while(uCurNode!=NULL){
		struct DLListNode* vCurNode = v->first;
		int flag = 0;
		while(vCurNode!=NULL){
			if(vCurNode->value==uCurNode->value) flag=1;
			vCurNode = vCurNode->next;
		}
		// the uCurNode not in result,insert it into result
		if(flag==0) {
			size++;
			DLListNode* temp = result->last;
			DLListNode* newNode = newDLListNode(uCurNode->value);
			temp->next = newNode;
			newNode->prev = temp;
			result->last = newNode;
		}
		uCurNode = uCurNode->next;
	}
	result->size = size;
	return result;
}

// compute the intersection of two DLLists u and v
// O(n^2)
DLList *setIntersection(struct DLList *u, struct DLList *v)
{	
	struct DLListNode* uCurNode = u->first;
	DLList* result = newDLList();
	struct DLListNode* firstNode = NULL;
	struct DLListNode* resCurNode = NULL;
	int count = 0;

	while(uCurNode!=NULL){
		struct DLListNode* vCurNode = v->first;
		while(vCurNode!=NULL){
			if(vCurNode->value==uCurNode->value){
				count++;
				if(count==1){
					firstNode = newDLListNode(uCurNode->value);
					resCurNode = firstNode;
				}else{
					struct DLListNode* temp = newDLListNode(uCurNode->value);
					resCurNode->next = temp;
					temp->prev = resCurNode;
					resCurNode = temp;
				}
			}
			vCurNode = vCurNode->next;
		}
		uCurNode = uCurNode->next;
	}
	result->first = firstNode;
	result->last = resCurNode;
	result->size = count;
	return result;
}

// free up all space associated with list
// O(n)
void freeDLList(struct DLList *L)
{
	struct DLListNode* curNode = L->first;
	while(curNode!=NULL){
		free(curNode);
		curNode = curNode->next;
	}
}


// display items of a DLList
// O(n)
void printDLList(struct DLList *u)
{
	struct DLListNode* curNode = u->first;
	while(curNode!=NULL){
		printf("%d ", curNode->value);
		curNode = curNode->next;
	}
	printf("\n");
}

int main()
{
 DLList *list1, *list2, *list3, *list4;

 list1=CreateDLListFromFileDlist("File1.txt");
 printDLList(list1);

 list2=CreateDLListFromFileDlist("File2.txt");
 printDLList(list2);

 list3=setUnion(list1, list2);
 printDLList(list3);

 list4=setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 printf("please type all the integers of list1\n");
 list1=CreateDLListFromFileDlist("stdin");

 printf("please type all the integers of list2\n");
 list2=CreateDLListFromFileDlist("stdin");

 list3=setUnion(list1, list2);
 printDLList(list3);
 list4=setIntersection(list1, list2);
 printDLList(list4);

 freeDLList(list1);
 freeDLList(list2);
 freeDLList(list3);
 freeDLList(list4);

 return 0; 
}
