#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#define Size 100
#define inf 1e50

// A vertex is a 2D point
typedef struct Vertex { 
	int x; // x-coordinate
	int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
	Vertex *p1; // first end point
	Vertex *p2; // second end point
} Edge;
typedef struct VertexNode {
	Vertex *v;
	int vis; // #whether in Queue
	double dist; // #distance from origin
	struct VertexNode *pre; // precursor vertex in mindist-path
	struct VertexNode *adj[Size]; // edge starting from v
	int nE; // #edges starting from v
} VertexNode;
typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
	VertexNode *vertices; // an array of vertices or a linked list of vertices  
	int nV; // #vertices
	int nE; // #edges
} GraphRep;
// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types. 
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment    

int cmpVertex(const void *u, const void *v)
{
	Vertex *a = (Vertex *) u;
	Vertex *b = (Vertex *) v;
	if (a->x != b->x)
		return (a->x) - (b->x);
	if (a->y != b->y)
		return (a->y) - (b->y);
	return 0; 
}

// Add the time complexity analysis of CreateEmptyGraph() here: O(Size)
Graph CreateEmptyGraph()
{
	Graph res = (Graph) malloc(sizeof (GraphRep));
	res->nE = res->nV = 0;
	res->vertices = (VertexNode *) malloc(sizeof (VertexNode) * Size);
	return res;
}
//Find appropriate node in g refers to p O(nV)
VertexNode *FindNode(Graph g, Vertex *p, int supplement)
{
	int i = 0; for (; i < g->nV ; i++)
		if (cmpVertex(g->vertices[i].v, p) == 0) return &(g->vertices[i]);
	if (!supplement) return NULL;
	int ind = g->nV ++;
	VertexNode *u = &(g->vertices[ind]);
	u->v = (Vertex *) malloc(sizeof (Vertex));
	*(u->v) = *p; u->vis = 0; u->nE = 0;
	return u;
}
//Find the index of edge p1->p2 in edge list O(nE)
int FindIndex(VertexNode *p1, VertexNode *p2)
{
	int i = 0; for (; i < p1->nE; i++)
		if (p1->adj[i] == p2) return i;
	return -1;
}

// Add the time complexity analysis of InsertEdge() here: O(nE + nV)
int InsertEdge(Graph g, Edge *e)
{
	VertexNode *p1 = FindNode(g, e->p1, 1);
	VertexNode *p2 = FindNode(g, e->p2, 1);
	if (FindIndex(p1, p2) != -1) return 0;
	int ind = p1->nE ++; p1->adj[ind] = p2;
	if (FindIndex(p2, p1) != -1) return 0;
	ind = p2->nE ++; p2->adj[ind] = p1;
	return 1;
}

// Add the time complexity analysis of DeleteEdge() here: O(nE + nV)
void DeleteEdge(Graph g, Edge *e)
{
	VertexNode *p1 = FindNode(g, e->p1, 0);
	VertexNode *p2 = FindNode(g, e->p2, 0);
	if (p1 == NULL || p2 == NULL) return ;
	int ind = FindIndex(p1, p2);
	if (ind == -1) return ;
	int i = ind; for (; i < p1->nE - 1; i++)
		p1->adj[i] = p1->adj[i + 1];
	p1->nE --;
	
	ind = FindIndex(p2, p1);
	if (ind == -1) return ;
	i = ind; for (; i < p2->nE - 1; i++)
		p2->adj[i] = p2->adj[i + 1];
	p2->nE --;
}

//bfs for searching O(nE + nV)
VertexNode *que[Size * Size]; 
int head, tail, tot; 
Vertex ans[Size];
void ReachableVertices(Graph g, Vertex *v)
{
	VertexNode *p = FindNode(g, v, 0);
	if (p == NULL) return ;
	que[head = tail = tot = 0] = p; p->vis = 1;
	while (head <= tail)
	{
		VertexNode *u = que[head ++];
		int i = 0; for (; i < u->nE ; i++)
			if (u->adj[i]->vis == 0)
			{
				que[++ tail] = u->adj[i];
				u->adj[i]->vis = 1;
				ans[tot ++] = *(u->adj[i]->v);
			}
	}
	qsort(ans, tot, sizeof(Vertex), cmpVertex);
	int i = 0; for (i = 0; i < g->nV; i++)
		g->vertices[i].vis = 0;
	for (i = 0; i < tot - 1; i++)
		printf("(%d,%d),", ans[i].x, ans[i].y);
	if (tot) 
		printf("(%d,%d)", ans[tot - 1].x, ans[tot - 1].y);
	printf("\n");
}

double Distance(Vertex *u, Vertex *v)
{
	return (double)sqrt ((double)(u->x - v->x) * (u->x - v->x) + (double)(u->y - v->y) * (u->y - v->y));
}

// SPFA for solving SP O(SPFA(nV, nE)) = O(nV * nE)
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
	VertexNode *p1 = FindNode(g, u, 0);
	VertexNode *p2 = FindNode(g, v, 0);
	if (u == NULL || v == NULL) return ;
	
	int i = 0; for (; i < g->nV; i++)
		g->vertices[i].dist = inf;
	que[head = tail = 0] = p1; p1->vis = 1; p1->dist = 0;
	while (head <= tail)
	{
		VertexNode *u = que[head ++]; u->vis = 0;
		int i = 0; for (; i < u->nE ; i++)
			if (u->adj[i]->dist > u->dist + Distance(u->v, u->adj[i]->v))
			{
				u->adj[i]->dist = u->dist + Distance(u->v, u->adj[i]->v);
				u->adj[i]->pre = u;
				if (u->adj[i]->vis == 0)
				{
					que[++ tail] = u->adj[i];
					u->adj[i]->vis = 1;
				}
			}
	}
	if (p2->dist >= inf) return ;
	tot = 0; VertexNode *path = p2;
	for (; path != p1; path = path->pre)
		ans[++tot] = *(path->v);
	printf("(%d,%d)", p1->v->x, p1->v->y);
	for (i = tot; i >= 1; i--)
		printf(",(%d,%d)", ans[i].x, ans[i].y);
	printf("\n");
}

// Add the time complexity analysis of FreeGraph() here: O(1)
void FreeGraph(Graph g)
{
	g->nV = g->nE = 0;
}

// bfs for searching and sort O(nE + nV log nV)
void ShowGraph(Graph g)
{
	VertexNode *p = &(g->vertices[0]);
	que[head = tail = tot = 0] = p; p->vis = 1;
	while (head <= tail)
	{
		VertexNode *u = que[head ++];
		int i = 0; for (; i < u->nE ; i++)
		{
			if (u->adj[i]->vis == 0)
			{
				que[++ tail] = u->adj[i];
				u->adj[i]->vis = tail + 1;
			}
			if (u->adj[i]->vis > u->vis)
				printf("(%d,%d),(%d,%d) ", u->v->x, u->v->y, u->adj[i]->v->x, u->adj[i]->v->y);
		}
	}
	int i = 0; for (; i < g->nV; i++)
		g->vertices[i].vis = 0;
	printf("\n");
}

int main() //sample main for testing 
{ 
	Graph g1;
	Edge *e_ptr; 
	Vertex *v1, *v2;
  
// Create an empty graph g1;
	g1=CreateEmptyGraph();
  
// Create first connected component 
// Insert edge (0,0)-(0,10)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=0;
	v2->y=10;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (0,0)-(5,6)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=5;
	v2->y=6;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (0, 10)-(10, 10)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=10;
	v2->x=10;
	v2->y=10;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (0,10)-(5,6)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=10;
	v2->x=5;
	v2->y=6;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (0,0)-(5,4)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=5;
	v2->y=4;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
 // Insert edge (5, 4)-(10, 4)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=5;
	v1->y=4;
	v2->x=10;
	v2->y=4;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (5,6)-(10,6)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=5;
	v1->y=6;
	v2->x=10;
	v2->y=6;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (10,10)-(10,6)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=10;
	v1->y=10;
	v2->x=10;
	v2->y=6;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (10, 6)-(10, 4)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=10;
	v1->y=6;
	v2->x=10;
	v2->y=4;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Create second connected component
// Insert edge (20,4)-(20,10)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=20;
	v1->y=4;
	v2->x=20;
	v2->y=10;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (20,10)-(30,10)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=20;
	v1->y=10;
	v2->x=30;
	v2->y=10;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
 
// Insert edge (25,5)-(30,10) 	
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=25;
	v1->y=5;
	v2->x=30;
	v2->y=10;
	e_ptr->p1=v1;
	e_ptr->p2=v2;
	if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n"); 
 
// Display graph g1
	ShowGraph(g1);
//	printf("ShowGraph1\n");
	
// Find the shortest path between (0,0) and (10,6) 
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=10;
	v2->y=6;
	ShortestPath(g1, v1, v2);
//	printf("ShortestPath1\n");
	free(v1);
	free(v2);
	  
// Delete edge (0,0)-(5, 6)
	e_ptr = (Edge*) malloc(sizeof(Edge));
	assert(e_ptr != NULL);
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=5;
	v2->y=6;
	e_ptr->p1=v1;
	e_ptr->p2=v2; 	 
	DeleteEdge(g1, e_ptr);
	free(e_ptr);
	free(v1);
	free(v2);
 	 
// Display graph g1
	ShowGraph(g1);
//	printf("ShowGraph2\n");
	
// Find the shortest path between (0,0) and (10,6) 
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=10;
	v2->y=6; 
	ShortestPath(g1, v1, v2);
//	printf("Shortest2\n");
	free(v1);
	free(v2);
 
// Find the shortest path between (0,0) and (25,5)
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v2=(Vertex *) malloc(sizeof(Vertex));
	assert(v2 != NULL);
	v1->x=0;
	v1->y=0;
	v2->x=25;
	v2->y=5;
	ShortestPath(g1, v1, v2);
//	printf("Shortest3\n");
	free(v1);
	free(v2);	
 
// Find reachable vertices of (0,0)
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x=0;
	v1->y=0;
	ReachableVertices(g1, v1);
//	printf("ReachableVertices1\n");
	free(v1);
 
// Find reachable vertices of (20,4)
	v1=(Vertex*) malloc(sizeof(Vertex));
	assert(v1 != NULL);
	v1->x=20;
	v1->y=4;
	ReachableVertices(g1, v1);
//	printf("ReachableVertices2\n");
	free(v1);
 
// Free graph g1
	FreeGraph(g1);
	 
	return 0; 
}
