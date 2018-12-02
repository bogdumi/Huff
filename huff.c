// Huffman encoding for image in BMP format

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// Stores RGB values of a pixel
struct pixel{
    unsigned char r, g, b;
};
typedef struct pixel pixel;

// Single node for Huffman tree
struct node{
	int freq;
	pixel data;
	struct node *left, *right;
};
typedef struct node node;

// Huffman tree structure
struct tree{
	unsigned int size, capacity;
	node **array;
};
typedef struct tree tree;

// BMP processing -------------------------------------------------------------

// Open the image to get pixel data
void openPixelsBMP(
	char *filename,
	unsigned int height,
	unsigned int width,
	pixel image[height][width]
	){

		FILE *f = fopen(filename, "rb");
		unsigned char rubbish;
		for(int i = 0; i < 54; i++){
			rubbish = fgetc(f);
		}
		for(int i = height - 1; i >= 0; i--){
   		    for(int j = 0; j < width; j++){
        	    image[i][j].b = fgetc(f);
            	image[i][j].g = fgetc(f);
            	image[i][j].r = fgetc(f);
        	}
    	}
    	fclose(f);
		return;
}

// Open image to get header data
void openHeaderBMP(char *filename, unsigned int *height, unsigned int *width){
    char BitmapHeader[14], BitmapInfoHeader[40];
    (*height) = 0;
    (*width) = 0;
    FILE *f = fopen(filename, "rb");
    for(int i = 0; i < 14; i++)
        BitmapHeader[i] = fgetc(f);
    for(int i = 0; i < 40; i++)
        BitmapInfoHeader[i] = fgetc(f);

    (*height) = (BitmapInfoHeader[11] << 24) |
    			(BitmapInfoHeader[10] << 16) |
    			(BitmapInfoHeader[9] << 8)   |
    			(BitmapInfoHeader[8]);
    (*width)  = (BitmapInfoHeader[7] << 24) |
    			(BitmapInfoHeader[6] << 16) |
    			(BitmapInfoHeader[5] << 8)  |
    			(BitmapInfoHeader[4]);


    fclose(f);
    return;
}

// Huffman encoding -----------------------------------------------------------

// Find frequencies of pixels
int findFreq(
	unsigned int height,
	unsigned int width,
	pixel image[height][width],
	pixel data[height * width],
	unsigned short frequency[256][256][256]
	){

		int d = 0;
		for(int i = 0; i < 256; i++)
			for(int j = 0; j < 256; j++)
				for(int k = 0; k < 256; k++)
					frequency[i][j][k] = 0;

		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++){
				if(!frequency[image[i][j].r][image[i][j].g][image[i][j].b]){
					data[d] = image[i][j];
					d++;
				}
				frequency[image[i][j].r][image[i][j].g][image[i][j].b]++;
			}
		return d;
}

// Allocate a new node
node *newNode(pixel data, unsigned short freq){
	node *n = malloc(sizeof(node));
	n -> data = data;
	n -> left = NULL;
	n -> right = NULL;
	n -> freq = freq;
	return n;
}

// Create tree function
tree *newTree(unsigned int capacity){
	tree *t = malloc(sizeof(tree));
	t -> size = 0;
	t -> capacity = capacity;
	t -> array = malloc(t -> capacity * sizeof(node));
	return t;
}

// Swap 2 nodes
void swapNodes(node **a, node **b){
	node *aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

// Finds minimum occurances between 2 nodes
void findMin(tree *t, int n){
	int smallest = n;
	int left = 2 * n + 1;
	int right = 2 * n + 2;

	if(left < t -> size && t -> array[left] -> freq < t -> array[smallest] -> freq)
		smallest = right;

	if(right < t -> size && t -> array[right] -> freq < t -> array[smallest] -> freq)
		smallest = left;

	if(smallest != n){
		swapNodes(&t -> array[smallest], &t -> array[n]);
		findMin(t, smallest);
	}
}

// Print codes to file
void printCode(unsigned int *array, int n, node *nd, FILE *f){
	
	char d[20] = "";
	int num = sprintf(d, "%d, %d, %d: ", nd -> data.r, nd -> data.g, nd -> data.b);
	if(num > 0){
		for(int i = 0; d[i] != '\0'; i++)
			fputc(d[i], f);
		for(int i = 0; i < n; i++)
			fputc(array[i] + '0', f);
		fputc('\n', f);
	}
}

// Check if we only have 1 node left
bool endNode(tree *t){
	return (t -> size == 1);
}

// Check if node is leaf
bool isLeaf(node *n){
	return (n -> left == NULL && n -> right == NULL);
}

// Get node with min occurances
node *getMin(tree *t){
	node *n = t -> array[0];
	t -> array[0] = t -> array[t -> size - 1];
	t -> size --;
	findMin(t, 0);
	return n;
}

// Insert a new node into tree
void insertNode(tree *t, node *n){
	t -> size ++;
	int i = t -> size - 1;
	while(i && n -> freq < t -> array[(i - 1) / 2] -> freq){
		t -> array[i] = t -> array[(i - 1) / 2];
		i = (i - 1) / 2;
	}
	t -> array[i] = n;
}

// Build a tree from bottom up
void buildTree(tree *t){
	int i, n = t -> size - 1;
	for(i = (n - 1) / 2; i >= 0; i--)
		findMin(t, i);
}

// Create tree, then build it
tree *createTree(pixel *data, unsigned short frequency[256][256][256], int size){
	tree *t = newTree(size);
	for(int i = 0; i < size; i++)
		t -> array[i] = newNode(data[i], frequency[data -> r][data -> g][data -> b]);
	t -> size = size;
	buildTree(t);
	return t;
}

// Huffman tree algorithm
node *huffmanTree(pixel *data, unsigned short frequency[256][256][256], int size){
	node *left, *right, *top;
	tree *t = createTree(data, frequency, size);
	pixel p;
	p.r = p.g = p.b = 0;

	while(!endNode(t)){
		left = getMin(t);
		right = getMin(t);
		top = newNode(p, left->freq + right->freq);

		top -> left = left;
		top -> right = right;

		insertNode(t, top);
	}
    return getMin(t);
}

// Print Huffman Codes given a root node
void printHuffCode(node *root, unsigned int *array, int top, FILE *f){
	if(root -> left){
		array[top] = 0;
		printHuffCode(root -> left, array, top + 1, f);
	}
	if(root -> right){
		array[top] = 1;
		printHuffCode(root -> right, array, top + 1, f);
	}
	if(isLeaf(root)){
		printCode(array, top, root, f);
	}
}

// Traverse the tree and print the Huffman Codes
void huffmanCode(pixel *data, unsigned short frequency[256][256][256], int size){
	node *root = huffmanTree(data, frequency, size);
	unsigned int array[1000 * 1000], top = 0;

	FILE *f = fopen("output.txt", "w");
	printHuffCode(root, array, top, f);
	fclose(f);
}

// Program startup
void startEncode(char *filename){
	unsigned int height = 0, width = 0;
	openHeaderBMP(filename, &height, &width);

	unsigned short (*frequency)[256][256];
    frequency = malloc(256 * sizeof(*frequency));

	pixel image[height][width], data[height * width];

	openPixelsBMP(filename, height, width, image);

	int uniquePixels = findFreq(height, width, image, data, frequency);

	huffmanCode(data, frequency, uniquePixels);

    free(frequency);
}

// Testing functions ----------------------------------------------------------

// Create a test BMP functions ------------------------------------------------
// Create Bitmap File Header
void createBitmapFileHeader(
	unsigned char *BitmapHeader,
	int height,
	int width,
	int filesize
	){
    	unsigned char BitmapHeaderT [14] = {
        	'B','M', // magic
        	0,0,0,0, // size in bytes
        	0,0, // app data
        	0,0, // app data
        	40+14,0,0,0 // start of data offset
    	};

    	BitmapHeaderT[2] = (unsigned char)(filesize);
    	BitmapHeaderT[3] = (unsigned char)(filesize >> 8);
    	BitmapHeaderT[4] = (unsigned char)(filesize >> 16);
    	BitmapHeaderT[5] = (unsigned char)(filesize >> 24);

    	for(int i = 0; i < 14; i++)
        	BitmapHeader[i] = BitmapHeaderT[i];
    	return;
}

// Create Bitmap Info Header
void createBitmapInfoHeader(
	unsigned char
	*BitmapInfoHeader,
	int height,
	int width,
	int datasize){
    	unsigned char BitmapInfoHeaderT [40] = {
        	40,0,0,0,
        	0,0,0,0,
        	0,0,0,0,
        	1,0,
        	24,0,
        	0,0,0,0,
        	0,0,0,0,
        	0x13,0x0B,0,0,
        	0x13,0x0B,0,0,
        	0,0,0,0,
        	0,0,0,0,
    	};

    	BitmapInfoHeaderT[4] = (unsigned char)(width);
    	BitmapInfoHeaderT[5] = (unsigned char)(width >> 8);
    	BitmapInfoHeaderT[6] = (unsigned char)(width >> 16);
    	BitmapInfoHeaderT[7] = (unsigned char)(width >> 24);

    	BitmapInfoHeaderT[8] = (unsigned char)(height);
    	BitmapInfoHeaderT[9] = (unsigned char)(height >> 8);
    	BitmapInfoHeaderT[10] = (unsigned char)(height >> 16);
    	BitmapInfoHeaderT[11] = (unsigned char)(height >> 24);

    	BitmapInfoHeaderT[20] = (unsigned char)(datasize);
    	BitmapInfoHeaderT[21] = (unsigned char)(datasize >> 8);
    	BitmapInfoHeaderT[22] = (unsigned char)(datasize >> 16);
    	BitmapInfoHeaderT[23] = (unsigned char)(datasize >> 24);

    	for(int i = 0; i < 40; i++)
        	BitmapInfoHeader[i] = BitmapInfoHeaderT[i];
    	return;
}

// Generate image main function
void generateBMP(int height, int width){
    pixel image[height][width];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j].r = (unsigned char)((double)i/height*255);
            image[i][j].g = (unsigned char)((double)j/width*255);
            image[i][j].b = (unsigned char)(((double)i+j)/(height+width)*255);
        }
    }

    FILE *f = fopen ("image.bmp", "wb");
    int padSize  = (4 - (width * 3) % 4) % 4;
    int filesize = width * height * 3 + 54;
    int datasize = width * height * 3 + height * padSize;

    unsigned char BitmapHeader[14];
    createBitmapFileHeader(BitmapHeader, height, width, filesize);
    unsigned char BitmapInfoHeader[40];
    createBitmapInfoHeader(BitmapInfoHeader, height, width, datasize);

    for(int i = 0; i < 14; i++)
        fputc(BitmapHeader[i], f);
    for(int i = 0; i < 40; i++)
        fputc(BitmapInfoHeader[i], f);

    for(int i = height - 1; i >= 0; i--){
        for(int j = 0; j < width; j++){
            fputc(image[i][j].b, f);
            fputc(image[i][j].g, f);
            fputc(image[i][j].r, f);
        }
    }
    fclose(f);
}

void generateCustomBMP(char *height, char *width){
	int h = atoi(height), w = atoi(width);
	if(w % 4 == 0)
		generateBMP(h,w);
	else
		printf("The width must be a multiple of 4.\n");
}

// Test openHeaderBMP
void testopenHeaderBMP(){
	unsigned int height, width;

	generateBMP(100,100);
	openHeaderBMP("image.bmp", &height, &width);
	assert(height == 100 && width == 100);

	generateBMP(800,100);
	openHeaderBMP("image.bmp", &height, &width);
	assert(height == 800 && width == 100);

	generateBMP(100,800);
	openHeaderBMP("image.bmp", &height, &width);
	assert(height == 100 && width == 800);

	generateBMP(600,600);
	openHeaderBMP("image.bmp", &height, &width);
	assert(height == 600 && width == 600);
}

// Test newNode
void testnewNode(){
	pixel p;
	p.r = 0;
	p.g = 0;
	p.b = 0;
	node *n1 = newNode(p, 10);
	assert (n1 != NULL);

	p.r = 100;
	p.g = 100;
	p.b = 100;
	node *n2 = newNode(p, 10);
	assert (n2 != NULL);

	p.r = 200;
	p.g = 200;
	p.b = 200;
	node *n3 = newNode(p, 10);
	assert (n3 != NULL);

	p.r = 255;
	p.g = 255;
	p.b = 255;
	node *n4 = newNode(p, 10);
	assert (n4 != NULL);
}

// Test newTree
void testnewTree(){
	tree *t1 = newTree(1);
	assert (t1 != NULL);

	tree *t2 = newTree(10);
	assert (t2 != NULL);

	tree *t3 = newTree(100);
	assert (t3 != NULL);

	tree *t4 = newTree(1000);
	assert (t4 != NULL);
}

// Test swapNodes
void testswapNodes(){
	pixel p;
	p.r = 0;
	p.g = 0;
	p.b = 0;
	node *n1 = newNode(p, 10);
	p.r = 100;
	p.g = 100;
	p.b = 100;
	node *n2 = newNode(p, 5);
	swapNodes(&n1, &n2);
	assert(n2 -> data.r == 0 && n2 -> data.g == 0 && n2 -> data.b == 0);
	assert(n1 -> data.r == 100 && n1 -> data.g == 100 && n1 -> data.b == 100);
}

// Test functions
void test(){
	testopenHeaderBMP();
	testnewNode();
	testnewTree();
	testswapNodes();
}

// Main function --------------------------------------------------------------
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) startEncode(args[1]);
    else if (n == 4 && !strcmp(args[1], "generate")) generateCustomBMP(args[2], args[3]);
    else printf("Use:   ./huff   or   ./huff (file)   or   ./huff generate (height) (width)\n");
    return 0;
}
