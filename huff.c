#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Testing functions ----------------------------------------------------------

// Create a test BMP ----------------------------------------------------------

// Create Bitmap File Header
unsigned char *createBitmapFileHeader(int height, int width, int filesize){
    unsigned char BitmapHeader[10] = {
        0, 0,       //bfType, must always be set to 'BM' to declare that this is a .bmp-file
        0, 0, 0, 0, //bfSize, specifies the size of the file in bytes
        0, 0, 0, 0, //bfReserved1,2 must always be set to zero
    }

    BitmapHeader[0] = (unsigned char) 'B';
    BitmapHeader[1] = (unsigned char) 'M';
    BitmapHeader[2] = (unsigned char) (filesize);
    BitmapHeader[3] = (unsigned char) (filesize >> 8);
    BitmapHeader[4] = (unsigned char) (filesize >> 16);
    BitmapHeader[5] = (unsigned char) (filesize >> 24);
    BitmapHeader[10] = (unsigned char) (54);

    return BitmapHeader;
}

unsigned char *createBitmapInfoHeader(int height, int width){

}


void generateBMP(int height, int width){
    FILE *f = fopen ("image.bmp", "wb");
    unsigned char *img = NULL;
    int filesize = 54 + 3 * width * height;
    unsigned char *BitmapHeader = createBitmapFileHeader(height, width, filesize);
    unsigned char *BitmapInfo = createBitmapInfoHeader(height, width, filesize);


    fclose(f);
}

// Test the Huffman encoding
void test(){

}

// Main function --------------------------------------------------------------
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) {

    }
    else printf("Use:   ./huff   or   ./huff (file)\n");
    return 0;
}