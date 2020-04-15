#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

// This template is only a guide
// You need to include additional fields, data structures and functions

// data type for heap nodes
typedef struct HeapNode {
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	int degree; // add additional fields here
	struct HeapNode *parent_ptr;
	struct HeapNode *sibling_ptr;
	struct HeapNode *child_ptr;

} HeapNode;

//data type for a priority queue (heap)
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	//... // add additional fields here
	HeapNode *header;
} BinomialHeap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d, int degree)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself)
	HeapNode *newNode;
	newNode = malloc(sizeof(HeapNode));
	assert(newNode != NULL);
	newNode->key = k;
	newNode->TaskName = n;
	newNode->Etime = c;
	newNode->Rtime = r;
	newNode->Dline = d;
	 // initialise other fields
	newNode->degree = degree;
	newNode->parent_ptr = NULL;
	newNode->sibling_ptr = NULL;
	newNode->child_ptr = NULL;

	return newNode;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	///... // initialise all the fields here
	T->size = 0;
	T->header = NULL;

	return T;
}

//Link two binomial heaps of same degree
//Time complexity O(1)
void BinomialHeapLink(HeapNode *T1, HeapNode *T2)
{
	assert(T1->degree == T2->degree);
	//Pick the one with larger key as the new root
	if (T2->key > T1->key)
	{
		T1->parent_ptr = T2;
		T1->sibling_ptr = T2->child_ptr;
		T2->child_ptr = T1;
		T2->degree++;
	}
	else
	{
		T2->parent_ptr = T1;
		T2->sibling_ptr = T1->child_ptr;
		T1->child_ptr = T2;
		T1->degree++;
	}
}

//Merge the root list of two binomial heap into 1
// with monotonically increasing order of the root degree
HeapNode* BinomialHeapMerge(BinomialHeap *T1, BinomialHeap *T2)
{
	HeapNode *temp = NULL;
	HeapNode *curr1 = T1->header;
	HeapNode *curr2 = T2->header;
	while (curr1 && curr2)
	{
		if (curr1->degree < curr2->degree)
		{
			temp->sibling_ptr = curr1;
			curr1 = curr1->sibling_ptr;

		}
		else
		{
			temp->sibling_ptr = curr2;
			curr2 = curr2->sibling_ptr;

		}
		temp = temp->sibling_ptr;
	}

	//Attach the remaining heap into temp
	if (curr1)
	{
		temp = curr1;
	}
	if (curr2)
	{
		temp = curr2;
	}

	return temp;
}

//Unite the two binomial heap
//Follow the pseudocode from CLRS Chapter 19
BinomialHeap* BinomialHeapUnion(BinomialHeap *T1, BinomialHeap *T2)
{
	BinomialHeap *binomialHeap = newHeap();
	HeapNode *prev_x;
	HeapNode *x;
	HeapNode *next_x;
	binomialHeap->header = BinomialHeapMerge(T1, T2);
	free(T1);
	free(T2);
	if (binomialHeap->header == NULL)
	{
		return NULL;
	}
	prev_x = NULL;
	x = binomialHeap->header;
	next_x = x->sibling_ptr;
	while (next_x != NULL)
	{
		if ((x->degree != next_x->degree) ||
			((next_x->sibling_ptr != NULL) && (next_x->sibling_ptr->degree == x->degree)))
		{
			prev_x = x;
			x = next_x;
		}
		else
		{
			if (x->key <= next_x->key)
			{
				x->sibling_ptr = next_x->sibling_ptr;
				BinomialHeapLink(next_x, x);
			}
			else
			{
				if (prev_x == NULL)
				{
					binomialHeap->header = next_x;
				}
				else
				{
					prev_x->sibling_ptr = next_x;
				}
				BinomialHeapLink(x, next_x);
				x = next_x;
			}

		}
		next_x = x->sibling_ptr;
	}


	return binomialHeap;
}


// put the time complexity analysis for Insert() here
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{	// k: key, n: task name, c: execution time, r: release time, d:deadline
	// You don't need to check if this task already exists in T
	//put your code here
	HeapNode* temp = newHeapNode(k, n, c, r, d, 0);
	if (T->header == NULL)
	{
		T->header = temp;
		T->size++;
		return;
	}
	else
	{

	}

}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
 // put your code here
    HeapNode *temp = NULL;

    return temp;
}


//Time complexity: O(logn)
//Check at most log(n) + 1 roots
int Min(BinomialHeap *T)
{
	// put your code here
	HeapNode *currNode = T->header;
	int minValue = INT_MAX;
	while (currNode != NULL)
	{
		if (currNode->key < minValue)
		{
			minValue = currNode->key;
		}
		currNode = currNode->sibling_ptr;
	}


	return minValue;
}


// put your time complexity analysis for MyTaskScheduler here
int TaskScheduler(char *f1, char *f2, int m )
{
 	// put your code here
	//Read in the files
	FILE *file1 = fopen(f1, "r");
	FILE *file2 = fopen(f2, "w");
	int v, c, r, d;
	int count = 0;
	if (file1 == NULL)
	{
		printf("Error! Could not open file 1\n");
		return -1;
	}
	if (file2 == NULL)
	{
		printf("Error! Could not open file 2\n");
		return -1;
	}

	while (fscanf(file1, " %d %d %d %d ", &v, &c, &r, &d) != EOF)
	{
		printf("Task %d %d %d %d \n", v, c, r, d);
		count++;
	}
    //Build the Release Time Binomial Heap


    //Build the Deadline Binomial Heap


    //Build the Scheduling point Binomial Heap

	fclose(file1);
	fclose(file2);

	return 0;
}

int main() //sample main for testing
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");

  return 0;
}
