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

const int MAX_TASK_SIZE = 10000;
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

HeapNode* BinomialHeapLink(HeapNode *T1, HeapNode *T2);
void print_helper(HeapNode *node, HeapNode *prev, int direction);
void PrintBinomialHeap(BinomialHeap *T);
HeapNode* BinomialHeapMerge(BinomialHeap *T1, BinomialHeap *T2);
BinomialHeap* BinomialHeapUnion(BinomialHeap *T1, BinomialHeap *T2);
BinomialHeap *Reverse(HeapNode* root);
BinomialHeap* Insert(BinomialHeap *T, int k, int n, int c, int r, int d);
HeapNode *RemoveMin(BinomialHeap **T);
int Min(BinomialHeap *T);
int TaskScheduler(char *f1, char *f2, int m );

//Link two binomial heaps of same degree
//Time complexity O(1)
HeapNode* BinomialHeapLink(HeapNode *T1, HeapNode *T2)
{
	assert(T1->degree == T2->degree);
	//Pick the one with smaller key as the new root
	if (T2->key <= T1->key)
	{
		T1->parent_ptr = T2;
		T1->sibling_ptr = T2->child_ptr;
		T2->child_ptr = T1;
		T2->degree++;
		return T2;
	}
	else
	{
		T2->parent_ptr = T1;
		T2->sibling_ptr = T1->child_ptr;
		T1->child_ptr = T2;
		T1->degree++;
		return T1;
	}
}

//Merge the root list of two binomial heap into 1
// with monotonically increasing order of the root degree
// Time complexity: O(m) with m is the total nodes of two heaps
HeapNode* BinomialHeapMerge(BinomialHeap *T1, BinomialHeap *T2)
{
	HeapNode *root = NULL;

	HeapNode ** pos = &root;
	HeapNode *curr1 = T1->header;
	HeapNode *curr2 = T2->header;

	while (curr1 && curr2)
	{
		if (curr1->degree < curr2->degree)
		{
			*pos = curr1;
			curr1 = curr1->sibling_ptr;

		}
		else
		{
			*pos = curr2;
			curr2 = curr2->sibling_ptr;

		}
		pos = &(*pos)->sibling_ptr;
	}

	//Attach the remaining heap into temp
	if (curr1)
	{
		*pos = curr1;
	}
	if (curr2)
	{
		*pos = curr2;
	}

	return root;
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
	//free(T1);
	//free(T2);
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
			if (x->key < next_x->key)
			{
				//Case 3
				x->sibling_ptr = next_x->sibling_ptr;
				x = BinomialHeapLink(next_x, x);
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
				x = BinomialHeapLink(x, next_x);
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
	int tempSize = T->size;
	HeapNode* newNode = newHeapNode(k, n, c, r, d, 0);
	BinomialHeap *tempHeap = newHeap();
	tempHeap->header = newNode;
	T = BinomialHeapUnion(T, tempHeap);
	T->size = tempSize + 1;
	return T;
}

// put your time complexity for RemoveMin() here
// This function returns the minimum node without modifying the heap in place
HeapNode *RemoveMin_v0(BinomialHeap *T)
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

	if (reversed_child == NULL)
	{
		reversed_child = newHeap();
	}
	T = BinomialHeapUnion(T, reversed_child);
	//PrintBinomialHeap(T);

    return minNode;
}

// Pass pointer to pointer T to modify the heap inplace
// The function returns the minimum node as well as modifies the heap in place
HeapNode *RemoveMin(BinomialHeap **T)
{
 // put your code here
    HeapNode *currNode = (*T)->header->sibling_ptr;
    HeapNode *prevNode = (*T)->header;
    HeapNode *minNode = (*T)->header;
    HeapNode *prevMinNode = NULL;
    int tempSize = (*T)->size;
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
		(*T)->header = (*T)->header->sibling_ptr;
	}
	else
	{
		// remove the min node from root list
		// prevMinNode -> minNode -> minNode_sibling
		prevMinNode->sibling_ptr = minNode->sibling_ptr;
	}

	BinomialHeap *reversed_child = newHeap();

	reversed_child = Reverse(minNode->child_ptr);
	if (reversed_child == NULL)
	{
		reversed_child = newHeap();
	}
	*T = BinomialHeapUnion(*T, reversed_child);
	//PrintBinomialHeap(T);
	if (*T == NULL)
	{
		*T = newHeap();
	}
	else
	{
		(*T)->size = tempSize - 1; //reduce by 1
	}

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
	int task_list[MAX_TASK_SIZE][4];
	int v, c, r, d;
	int flag = -1;
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

	while (1)
	{
		flag = fscanf(file1, " %d %d %d %d ", &v, &c, &r, &d);
		if (flag == EOF)
		{
			break;
		}
		if (flag < 4)
		{
			printf("Invalid inputs\n");
			return -1;
		}

		task_list[count][0] = v;
		task_list[count][1] = c;
		task_list[count][2] = r;
		task_list[count][3] = d;
		count++;
		//if any value is less than 0 then there is an invalid input
		if ((v < 0) || (c <= 0) || (r < 0) || (d <= 0))
		{
			printf("Invalid negative inputs\n");
			return -1;
		}
	}
	//print task list for verification
	/*
	for (int i = 0; i < count; i++)
	{
		printf("Task %d %d %d %d \n", task_list[i][0], task_list[i][1], task_list[i][2], task_list[i][3]);
	}
	*/


    //Build the Release Time Binomial Heap
	BinomialHeap *ReleaseTimeHeap = newHeap();

	for(int i = 0; i < count; i++)
	{
		//BinomialHeap* Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
		//printf("Task %d %d %d %d \n", task_list[i][0], task_list[i][1], task_list[i][2], task_list[i][3]);
		v = task_list[i][0]; //task name
		c = task_list[i][1]; //exe time
		r = task_list[i][2]; //release time
		d = task_list[i][3]; //deadline
		ReleaseTimeHeap = Insert(ReleaseTimeHeap, r, v, c, r, d);
	}


	//Build the Scheduling point Binomial Heap
	BinomialHeap *CoreHeap = newHeap();
	int tempMinRelease = Min(ReleaseTimeHeap);
	//printf("tempMinRelease %d\n", tempMinRelease);
	for(int j = 1; j <= m; j++ )
	{
		CoreHeap = Insert(CoreHeap, tempMinRelease, j, 0, 0, 0);
	}

    //Build the Deadline Binomial Heap
    HeapNode *node;
    BinomialHeap *DeadlineHeap = newHeap();
    int s1, s2, s; // scheduling points

    while (ReleaseTimeHeap->size > 0)
    {
		s1 = Min(ReleaseTimeHeap); //pick the smallest release time
		s2 = Min(CoreHeap); // pick the closest core available
		s = (s1 >= s2) ? s1 : s2; // schedule based on the larger one
		printf("size: %d\n", ReleaseTimeHeap->size);
		while (Min(ReleaseTimeHeap) <= s)
		{
			//PrintBinomialHeap(ReleaseTimeHeap);
			node = RemoveMin(&ReleaseTimeHeap); //take out nodes with smallest release time
			// Build up the Deadline Heap with key is the deadline
			//printf("After remove min\n");
			//PrintBinomialHeap(ReleaseTimeHeap);
			printf("node: %d %d %d %d\n", node->TaskName, node->Etime, node->Rtime, node->Dline);
			DeadlineHeap = Insert(DeadlineHeap,
									node->Dline,
									node->TaskName,
									node->Etime,
									node->Rtime,
									node->Dline);
		}

		while (DeadlineHeap->size > 0)
		{
			HeapNode *node_core;
			node_core = RemoveMin(&CoreHeap);
			int core = node_core->TaskName;

			HeapNode *deadline_node;
			deadline_node = RemoveMin(&DeadlineHeap);
			int f = ( deadline_node->Rtime >= node_core->key ) ? deadline_node->Rtime : node_core->key;
			CoreHeap = Insert(CoreHeap, f + deadline_node->Etime, core, 0,0,0);
			fprintf(file2, "%d Core%d %d\n",deadline_node->TaskName, core, f);
			if(deadline_node->key < f + deadline_node->Etime)
			{
				fprintf(file2, "where task%d misses its deadline.\n", deadline_node->TaskName);
                return 0;
			}

			while (Min(ReleaseTimeHeap) <= Min(CoreHeap))
			{
				node = RemoveMin(&ReleaseTimeHeap);
				DeadlineHeap = Insert(DeadlineHeap,
										node->Dline,
										node->TaskName,
										node->Etime,
										node->Rtime,
										node->Dline);
			}
		}

    }




	fclose(file1);
	fclose(file2);

	return 1;
}

void print_helper(HeapNode *node, HeapNode *prev, int direction)
{
	//node is current node
	//prev is either parent node or previous sibling
	//direction: 1 - current node is the left-most child
	//           2 - current node is one of the sibling
	while (node != NULL)
	{
		if (direction == 1)
		{
			printf("node\t %d (%d) is child of %d\n", node->key, node->degree, prev->key);
		}
		else
		{
			printf("node\t %d (%d) is next sibling of %d\n", node->key, node->degree, prev->key);
		}
		if (node->child_ptr != NULL)
		{
			print_helper(node->child_ptr, node, 1);
		}
		prev = node;
		node = node->sibling_ptr;
		direction = 2;
	}
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
	printf("\n");
	int i = 0;
	node = T->header;
	while (node)
	{
		i++;
		printf("B%d:\n",node->degree);
		printf("root:\t %d (%d)\n", node->key, node->degree);
		print_helper(node->child_ptr, node, 1);
		node = node->sibling_ptr;
	}

	printf("\n");

}

int main() //sample main for testing
{
	int i;
	//i=TaskScheduler("samplefile0.txt", "feasibleschedule0.txt", 4);
	//if (i==0) printf("No feasible schedule!\n");

	i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4 );
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


	//Test binomial heaps helper functions
	/*
	BinomialHeap *myHeap = newHeap();
	myHeap = Insert(myHeap, 10, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 1, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 12, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 12, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 12, 0, 0, 0, 0);

	myHeap = Insert(myHeap, 6, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 8, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 14, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 29, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 11, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 17, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 38, 0, 0, 0, 0);
	myHeap = Insert(myHeap, 27, 0, 0, 0, 0);

	printf("Total node: %d\n", myHeap->size);
	PrintBinomialHeap(myHeap);
	printf("Min value: %d\n", Min(myHeap));
	printf("Min node value: %d\n", RemoveMin(&myHeap)->key);
	printf("Heap after extract min value: \n");
	PrintBinomialHeap(myHeap);
	*/
	return 0;
}
