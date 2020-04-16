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
// Time complexity: O(m) with m is the total nodes of two heaps
HeapNode* BinomialHeapMerge(BinomialHeap *T1, BinomialHeap *T2)
{
	HeapNode *temp = malloc(sizeof(HeapNode));

	HeapNode *curr1 = T1->header;
	HeapNode *curr2 = T2->header;
	while (curr1 && curr2)
	{
		if (curr1->degree < curr2->degree)
		{
			temp = curr1;
			curr1 = curr1->sibling_ptr;

		}
		else
		{
			temp = curr2;
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
// Time complexity: O(log(n)) with n is the total nodes of two heaps
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
			//Case 1 and 2
			prev_x = x;
			x = next_x;
		}
		else
		{
			if (x->key <= next_x->key)
			{
				//Case 3
				x->sibling_ptr = next_x->sibling_ptr;
				BinomialHeapLink(next_x, x);
			}
			else
			{
				//Case 4
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


BinomialHeap *Reverse(HeapNode* root)
{
	//Input root is the left most child of the removed minimum node
	BinomialHeap *reversedHeap = newHeap();
	HeapNode *next;
	HeapNode *tail = NULL;
	if (root == NULL)
		return NULL;
	root->parent_ptr = NULL;
	while (root->sibling_ptr)
	{
		next = root->sibling_ptr;
		root->sibling_ptr = tail;
		tail = root;
		root = next;
		root->parent_ptr = NULL;
	}
	root->sibling_ptr = tail;

	reversedHeap->header = root;

	return reversedHeap;
}


// put the time complexity analysis for Insert() here
// Time complexity: O(log(n)) for the binomial heap union function
BinomialHeap* Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{	// k: key, n: task name, c: execution time, r: release time, d:deadline
	// You don't need to check if this task already exists in T
	//put your code here
	HeapNode* newNode = newHeapNode(k, n, c, r, d, 0);
	BinomialHeap *tempHeap = newHeap();
	tempHeap->header = newNode;
	T = BinomialHeapUnion(T, tempHeap);
	return T;
}

// put your time complexity for RemoveMin() here
HeapNode *RemoveMin(BinomialHeap *T)
{
 // put your code here
    HeapNode *currNode = T->header->sibling_ptr;
    HeapNode *prevNode = T->header;
    HeapNode *minNode = T->header;
    HeapNode *prevMinNode = NULL;
    //Find the minimum node (root)
    while (currNode)
    {
    	if (currNode->key < minNode->key)
    	{
    		minNode = currNode;
    		prevMinNode = prevNode;
    	}
    	prevNode = currNode;
    	currNode = currNode->sibling_ptr;
    }
	// Remove the minimum root
	if (prevMinNode == NULL)
	{
		//root header 1st node is the minimum
		T->header = T->header->sibling_ptr;
	}
	else
	{
		// remove the min node from root list
		// prevMinNode -> minNode -> minNode_sibling
		prevMinNode->sibling_ptr = minNode->sibling_ptr;
	}

	BinomialHeap *reversed_child = newHeap();

	reversed_child = Reverse(minNode->child_ptr);

	T = BinomialHeapUnion(T, reversed_child);


    return minNode;
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
		//if any value is less than 0 then there is an invalid input
		if ((v < 0) || (c < 0) || (r < 0) || (d < 0))
		{
			printf("Invalid negative inputs\n");
			return -1;
		}
	}
    //Build the Release Time Binomial Heap


    //Build the Deadline Binomial Heap


    //Build the Scheduling point Binomial Heap

	fclose(file1);
	fclose(file2);

	return 0;
}


void PrintBinomialHeap(BinomialHeap *T)
{
	HeapNode *node = T->header;
	if (node == NULL)
	{
		return;
	}
	printf("Binomial Heap: ");
	while (node)
	{
		printf(" B%d ", node->degree);
		node = node->sibling_ptr;
	}


}

int main() //sample main for testing
{
	//int i;
	//i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
	//if (i==0) printf("No feasible schedule!\n");


	//Test binomial heaps helper functions
	BinomialHeap *myHeap = newHeap();
	myHeap = Insert(myHeap, 10, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 1, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 12, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 25, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 18, 0, 0, 0, 0);

	myHeap = Insert(myHeap, 6, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 8, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 14, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 29, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 11, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 17, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 38, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 27, 0, 0, 0, 0);

	PrintBinomialHeap(myHeap);
	return 0;
}
