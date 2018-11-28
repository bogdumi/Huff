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

// Allocate a new node
node *newNode(pixel data, unsigned int freq){
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

// Build a tree
void buildTree(tree *t){
	int i, n = t -> size - 1;
	for(i = (n - 1) / 2; i >= 0; i--)
		findMin(t, i);
}

// Create tree, then build it
tree *createTree(int ***data, int ***frequency, int size){
	
}

// Program startup
void startEncode(char *filename){
	unsigned int height = 0, width = 0;
	pixel image[1100][1100];
	openHeaderBMP(filename, &height, &width);
	openPixelsBMP(filename, height, width, image);

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
        	40,0,0,0, // info hd size
        	0,0,0,0, // width
        	0,0,0,0, // height
        	1,0, // number color planes
        	24,0, // bits per pixel
        	0,0,0,0, // compression is none
        	0,0,0,0, // image bits size
        	0x13,0x0B,0,0, // horz resoluition in pixel / m
        	0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        	0,0,0,0, // #colors in pallete
        	0,0,0,0, // #important colors
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
            image[i][j].r = (unsigned char)((double)i/height*255);             // red
            image[i][j].g = (unsigned char)((double)j/width*255);              // green
            image[i][j].b = (unsigned char)(((double)i+j)/(height+width)*255); // blue
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

	generateBMP(1100,1100);
	openHeaderBMP("image.bmp", &height, &width);
	assert(height == 1100 && width == 1100);
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

// Test the Huffman encoding
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
    else printf("Use:   ./huff   or   ./huff (file)\n");
    return 0;
}
