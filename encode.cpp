#include "encode.hpp"
#include "headers.hpp"

// stores code for all characters
codes allcodes[256];

// total no of distinct characters
int unique_char_count;

// total no of bits in compressed file
int number_count = 0;

// total no of characaters in uncompressed file
int alphabet_count = 0;

// total no of characters in code map
int code_char_count = 0;

// ********************************************
// Menu functions
// ********************************************

// the encode menu
void encode() {
	encode_menu();
}

void encode_menu() {
	int ch;
	while (true) {
	        cout<<"\n\n===========>\tEncode Menu\t<===========\n"<<endl;
	        cout<<"0. Go back to main menu"<<endl;
	        cout<<"1. Encode existing file"<<endl;
	        cout<<"\nEnter your choice: ";
	        cin>>ch;
	        if (!ch) {
	        	cout<<"\nGoing back to main menu..."<<endl;
	        	break;
	        }
	        switch (ch) {
	        	case 1:
	        		encode_file();
	        		break;
	        	default:
	        		cout<<"\nEnter the right choice (0-1)."<<endl;
	       	}
	}
}

void encode_file() {
	cout<<"\n\n=====>\tEncode File\t<=====\n"<<endl;

	char file_name[100];
	cout<<"Enter the file name: ";
	cin>>file_name;

	FILE *fp;

	fp = fopen(file_name, "r");
	if (!fp) {
	        cout<<"\nGiven file does not exist."<<endl;
	        cout<<"Create a new file or use an existing file in the same directory"<<endl;
	        return;
	}
	cout<<"\nFile successfully opened."<<endl;
	// Reading file contents
	cout<<"Reading file contents...\n"<<endl;
	int c = 0;
	char ch;
	while ((ch = fgetc(fp)) != EOF) {
	        cout<<ch;
	        c++;
	}
	fclose(fp);
	cout<<"\nFile read complete."<<endl;
	if (c == 0) {
		cout<<"\nNo characters in file...."<<endl;
	        cout<<"Enter a file with content or enter content to a new file"<<endl;
	        return;
	}

	// File size
	cout<<"Total no of characters: "<<c<<endl;
	cout<<"File size: "<<c<<" bytes\n"<<endl;
	// Confirm compression
	encode_confirm(file_name);
}

// confirm the encoding process
void encode_confirm(char *file_name) {
	char choice = 'y';
	while (choice != 'N') {
		cout<<"\nDo you want to continue with the compression (Y/N)?: ";
	        cin>>choice;
	        if (choice == 'Y' || choice == 'y') {
		        encode_begin(file_name);
        		return;
	        } else if (choice == 'N' || choice == 'N') {
        		cout<<"\nGoing back to encode menu..."<<endl;
        		break;
        	} else {
		        cout<<"\nEnter right input."<<endl;
	        }
    	}
}

// ********************************************
// Helper functions
// ********************************************

// Print binary tree in tree form

int rec[1000006];
// print tree in tree form
void print_tree(struct Node *curr, int depth) {
	int i;
	if (curr == NULL)
		return;
	cout<<"\t";
	for (i = 0; i < depth; i++)
		if (i == depth - 1) {
        		cout << (rec[depth - 1] ? "l---" : "l---");
    		} else {
        		cout << (rec[i] ? "l   " : "    ");
    		}
	cout<<curr->freq<<endl;
	rec[depth] = 1;
	print_tree(curr->left, depth + 1);
	rec[depth] = 0;
	print_tree(curr->right, depth + 1);
}
// wrapper function to print tree
void print_huffman_tree(Node *root) {
	cout<<"\n\nHuffman Tree: "<<endl;
	print_tree(root, 0);
}

// create a node
Node *create_node(int f, char c) {
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->freq = f;
	temp->character = c;
	temp->right = NULL;
	temp->left = NULL;
	temp->parent = NULL;
	return temp;
}
// check if node is a leaf node
int is_leaf(struct Node *root) {
	return !(root->left) && !(root->right);
}

// swap 2 nodes
void swap_nodes(struct Node **a, struct Node **b) {
	struct Node *t = *a;
	*a = *b;
	*b = t;
}

// print queue
void print_queue(struct MinHeap *minHeap) {
	cout<<"\nCurrent Queue: ";
	for (int i = 0; i < minHeap->size; i++) {
		cout<<minHeap->array[i]->freq<<" ";
	}
	cout<<endl;
}

// ********************************************
// Encoding functions
// ********************************************

// All function calls for encoding takes place here
void encode_begin(char *file_name) {
	MinHeap *minHeap;
	Node *root;
	int arr[256];

	cout<<"\n\n=====>\tEncoding File\t<=====\n"<<endl;
	cout<<"File name: "<<file_name<<endl;

	encode_frequency(file_name);

	minHeap = build_heap();

	root = encode_huffman_tree(minHeap);

	print_huffman_tree(root);

	root = splay_nodes(root);

	print_tree(root, 0);

	reach_leaf_nodes(root, arr, 0);

	print_all_codes();

	encode_to_files(file_name);

	encode_done();
}

// print huffman codes
void print_all_codes() {
	cout<<"\nCorresponding Huffman code for character:"<<endl;
	int counter=0;
	for (int i = 0; i < 256; i++) {
	        if (allcodes[i].character != '\0') {
		        cout<<"\n"<<allcodes[i].character<<" - "<<allcodes[i].code;
		        counter++;
		        if (counter%10==0)
		        	sleep(1);
        	}
	}
	cout<<endl;
}

Node *unique2[256];
map<char, int> uq; // # unique
// Calculate frequency of all characters
void encode_frequency(char *file_name) {

	FILE *fp;
	fp = fopen(file_name, "r");

	unique_char_count = 0;

	char ch;
	while ((ch = fgetc(fp)) != EOF) {
	        uq[ch]++;
	}
	fclose(fp);

	cout<<"\nThe hashmap of characters and their frequencies are:"<<endl;
	int counter=0;
	for (auto [k, v] : uq) {
	        unique2[unique_char_count] = create_node(1, k);
	        unique_char_count++;
	        cout << "\n" << k << " - " << v;
	        counter++;
	        if (counter%10==0)
	        	sleep(1);
    	}
    	cout<<endl;
}

MinHeap *splayHeap = nullptr;

// function to build the heap
MinHeap *build_heap() {

	splayHeap = create_min_heap(unique_char_count);

	for (int i = 0; i < unique_char_count; ++i) {
	        splayHeap->array[i] = unique2[i];
	}

	splayHeap->size = unique_char_count;
	build_min_heap(splayHeap);

	cout<<endl;

	return splayHeap;
}

// function to create the minheap
struct MinHeap *create_min_heap(int capacity) {
	struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = (struct Node **)malloc(minHeap->capacity * sizeof(struct Node *));
	return minHeap;
}

// heapify
void heapify(struct MinHeap *minHeap, int x) {
	int least = x;
	int left = 2 * x + 1;
	int right = 2 * x + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[least]->freq)
	        least = left;

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[least]->freq)
		least = right;

	if (least != x) {
	        swap_nodes(&minHeap->array[least], &minHeap->array[x]);
	        heapify(minHeap, least);
	}
}

// build min heap
void build_min_heap(MinHeap *minHeap) {
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		heapify(minHeap, i);
}

// function to make the huffman tree
Node *encode_huffman_tree(MinHeap *minHeap) {

	cout<<"\nBuilding the huffman tree\n"<<endl;
	Node *left, *right, *top;
	print_queue(minHeap);
	while (minHeap->size != 1) {
		left = pop(minHeap);
	        right = pop(minHeap);

	        cout<<"Popped nodes frequency: "<<left->freq<<", "<<right->freq<<endl;

		top = create_node(left->freq + right->freq, '\0');
	        cout<<"New node frequency: "<<top->freq<<endl;
	        sleep(1);
        	top->left = left;
        	top->right = right;
        	left->parent = top;
        	right->parent = top;
        	insert_heap(minHeap, top);
        	print_queue(minHeap);
    	}
	return pop(minHeap);
}

// function to pop from queue
struct Node *pop(struct MinHeap *minHeap) {
	struct Node *temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];
	--minHeap->size;
	heapify(minHeap, 0);

	return temp;
}

// function to insert new node to heap
void insert_heap(struct MinHeap *minHeap, struct Node *minHeapNode) {
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
	        minHeap->array[i] = minHeap->array[(i - 1) / 2];
	        i = (i - 1) / 2;
	}
	minHeap->array[i] = minHeapNode;
}

Node *rightRotate(Node *x) {
	Node *y = x->left;
	x->left = y->right;
	y->right = x;
	return y;
}
Node *leftRotate(Node *x) {
	Node *y = x->right;
	x->right = y->left;
	y->left = x;
	return y;
}
Node *splay(Node *root, char character) {
	if (root == NULL || root->character == character)
	        return root;

	if (root->character > character) {
		if (root->left == NULL)
		        return root;

		if (root->left->character > character) {
		        root->left->left = splay(root->left->left, character);
		        root = rightRotate(root);
	        } else if (root->left->character < character) {
		        root->left->right = splay(root->left->right, character);
		        if (root->left->right != NULL)
		                root->left = leftRotate(root->left);
	        }
	        return (root->left == NULL) ? root : rightRotate(root);
	} else {
	        if (root->right == NULL)
		        return root;
        	if (root->right->character > character) {
        		root->right->left = splay(root->right->left, character);
		        if (root->right->left != NULL)
	 	               root->right = rightRotate(root->right);
        	} else if (root->right->character < character) {
		        root->right->right = splay(root->right->right, character);
		        root = leftRotate(root);
        	}
        	return (root->right == NULL) ? root : leftRotate(root);
    	}
}

// function to splay the nodes according to the frequency
Node *splay_nodes(Node *node) {

    // insert the characters to a vector
	vector<char> characters;
	for (auto x : uq) {
	        characters.push_back(x.first);
	}

    // sort characters based on map's second value
	sort(characters.begin(), characters.end(), [](const char &a, const char &b) { return uq[a] < uq[b]; });

	for (auto x : characters) {
	        // splay the parent of the char
	        // cout<<"Splaying the parent of "<<x<<endl;
	        node = splay(node, x);
	}

	return node;
}

// function to reach the leaf nodes
void reach_leaf_nodes(struct Node *root, int arr[], int top) {
	if (root->left) {
	        arr[top] = 0;
	        reach_leaf_nodes(root->left, arr, top + 1);
	}
    	if (root->right) {
        	arr[top] = 1;
        	reach_leaf_nodes(root->right, arr, top + 1);
    	}
    	if (is_leaf(root)) {
    		encode_map(arr, top, root->character);
    	}
}

// function to encode the map
void encode_map(int arr[], int n, char c) {
	int i;
	int ch = c;
	char temp[10] = "";
	for (i = 0; i < n; ++i) {
		if (arr[i] == 1) {
		        strcat(temp, "1");
        	}
	        if (arr[i] == 0) {
		        strcat(temp, "0");
        	}
    	}
	strcat(temp, "\0");
	strcpy(allcodes[ch].code, temp);
	allcodes[ch].character = c;
}

int current_bit = 0;
unsigned char bit_buffer;
FILE *f;

// function to write to text and binary files
void encode_to_files(char *file_name) {
	char encoded_file_name[120] = "compressed.txt";
	char codes_filename[120] = "codes.txt";

	char encoded_file_name_b[120] = "compressed.dat";
	char codes_filename_b[120] = "codes.dat";

	cout<<"\nEncoding to files..."<<endl;
	sleep(1);
	cout<<"Encoded file name: "<<encoded_file_name_b<<endl;
	sleep(1);
	cout<<"Code map for decoding present in: "<<codes_filename_b<<endl;
	sleep(1);
	cout<<"The visual reprsentation of the binary files can be found in the below text files."<<endl;
	cout<<"Encoded text file name: "<<encoded_file_name<<endl;
	cout<<"Code map for viewing present in: "<<codes_filename<<endl;
	sleep(1);

	FILE *fp, *qp, *rp, *rbp;

	fp = fopen(file_name, "r");
	f = fopen(encoded_file_name_b, "wb");
	qp = fopen(encoded_file_name, "w");

	char ch;
	int c;
	unsigned char byte = 0;
    	int bits_in_byte = 0;

    	while ((ch = fgetc(fp)) != EOF) {
            	c = ch;
            	fprintf(qp, "%s", allcodes[c].code);
            	alphabet_count++;

            	for (int i = 0; allcodes[c].code[i] != '\0'; i++) {
                    	number_count++;
                    	if (allcodes[c].code[i] == '1') {
                            	byte |= (1 << (7 - bits_in_byte));
                    	}
                    	bits_in_byte++;
                    	if (bits_in_byte == 8) {
                            fwrite(&byte, 1, 1, f);
                            byte = 0;
                            bits_in_byte = 0;
                    	}
            	}
    	}
    	if (bits_in_byte != 0) {
            	while (bits_in_byte < 8) {
                    	byte <<= 1;
                    	bits_in_byte++;
            	}
            	fwrite(&byte, 1, 1, f);
    	}
	fclose(fp);
	fclose(qp);
	fclose(f);

	cout<<"\nEncoded file contents: \n"<<endl;
	qp = fopen(encoded_file_name, "r");
	while ((ch = fgetc(qp)) != EOF) {
		cout<<ch;
		c++;
	}
	cout<<endl;
	fclose(qp);

	rbp = fopen(codes_filename_b, "wb");
	rp = fopen(codes_filename, "w");

	fprintf(rp, "%s", "Corresponding Huffman code for character : \n");
	char temp; // used to store character
	fwrite(&number_count, sizeof(number_count), 1, rbp);
	code_char_count += sizeof(number_count);
	for (int i = 0; i < 256; i++) {
		temp = allcodes[i].character;
	        if (temp != '\0') {
		        fwrite(&temp, sizeof(temp), 1, rbp);
		        code_char_count += 1;
		        for (int j = 0; allcodes[i].code[j] != '\0'; j++) {
			        fwrite(&allcodes[i].code[j], sizeof(allcodes[i]).code[j], 1, rbp);
		                code_char_count += 1;
		        }
		        fprintf(rp, "\n%c - %s", temp, allcodes[i].code);
		}
	}
	cout<<endl;
	fclose(rp);
	fclose(rbp);
}

// function to wait for 8 bits before writitng a byte to a file
void WriteBit(int bit) {
	bit_buffer <<= 1;
	if (bit)
		bit_buffer |= 0x1;

	current_bit++;
	if (current_bit == 8) {
	        fwrite(&bit_buffer, 1, 1, f);
	        current_bit = 0;
	        bit_buffer = 0;
	}
}

// function to flush the remaining bits
void Flush_Bits(void) {
	while (current_bit)
		WriteBit(0);
}

// indicates finishing of the process and prints the summary
void encode_done() {
	cout<<"\n===>Encoding Summary<==="<<endl;
	cout<<"Total no of characters in the file: "<<alphabet_count<<endl;
	sleep(1);
	cout<<"Total no of bits in the uncompressed file: "<<alphabet_count * 8<<endl;
	sleep(1);
	cout<<"Total no of bits in the compressed file: "<<number_count<<endl;
	sleep(1);
	cout<<"Total no of bits in code map: "<<code_char_count<<endl;
	sleep(1);
	cout<<"Total no of bits after compression: "<<number_count<<" + "<<code_char_count<<" = "<<number_count + code_char_count<<endl;
	sleep(1);
	cout<<"\nEncoding completed successfully...."<<endl;
}
