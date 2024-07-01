#ifndef ENCODE_H
#define ENCODE_H

struct Node {
	struct Node *left;
	struct Node *right;
	int freq;       // frequency of character in file
	char character; // the character assigned to this node
	struct Node *parent;
};
typedef struct Node Node;

struct MinHeap {
	int size;
	int capacity;
	struct Node **array;
};
typedef struct MinHeap MinHeap;

struct codes {
	char character;
	char code[10];
};
typedef struct codes codes;

// function to access all the encode functions
void encode();

// menu driven function to get new input/use old file/exit
void encode_menu();

// function to get input and create new file
void encode_input();

// function to use existing file
void encode_file();

// function to confirm the encoding process
void encode_confirm(char *filename);

// function to print binary tree
void print_tree();

// Start the encoding process
void encode_begin(char *filename);

// count the frequency
void encode_frequency(char *filename);

// function to build the heap
MinHeap *build_heap();

// function to create min heap
MinHeap *create_min_heap(int capacity);

// helper function to swap two nodes
void swap_nodes();

// function to heapify an array
void heapify();

// function that builds the min heap from the nodes
void build_min_heap(MinHeap *min_heap);

// function to build the huffman tree
Node *encode_huffman_tree(MinHeap *min_heap);

// function to splay the nodes according to the frequency
Node *splay_nodes(Node *node);

// splay the node
Node *splay(Node *node, char character);

// right and left rotate
Node *rightRotate(Node *node);
Node *leftRotate(Node *node);

// wrapper function to print the huffman tree
void print_huffman_tree();

// function to pop from the queue
Node *pop(MinHeap *minHeap);

// function to insert into the heap
void insert_heap(MinHeap *minHeap, struct Node *minHeapNode);

// function to reach leaf nodes
void reach_leaf_nodes(struct Node *root, int arr[], int top);

// function to encode the map
void encode_map(int *, int, char);

// function to check if node is a leaf node
int is_leaf();

// function to print tree in tree format
void print_tree();

// function to print the queue
void print_queue();

// function to print all codes
void print_all_codes();

// function to encode the file
void encode_to_files(char *filename);

// function to wait for 8 bits before writing byte to file
void WriteBit(int);

// function to flush remaining bits
void Flush_Bits();

// indicates finishing of the process
void encode_done();

#endif

// sample inputs
// ABBCDBCCDAABBEEEBEAB
// BCCABBDDAECCBBAEDDCC
