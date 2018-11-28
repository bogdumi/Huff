#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct pixel{
    unsigned char r,g,b;
};
typedef struct pixel pixel;

// Open image to get data
void openBMP(char *filename, pixel image[541][800], int *height, int *width){
    char file[14], info[40];
    (*height) = 0;
    (*width) = 0;
    FILE *f = fopen(filename, "rb");
    for(int i = 0; i < 14; i++)
        file[i] = fgetc(f);
    for(int i = 0; i < 40; i++)
        info[i] = fgetc(f);
    
    char heightS[4] = "1", widthS[4] = "1";
    strcpy(heightS, info + 23);
    strcpy(widthS, info + 19);

    for(int i = 0; i < 4; i++){
        (*height) = (*height) * 10 + heightS[i];
        (*width) = (*width) * 10 + widthS[i];
    }

    printf("%d %d\n", (*height), (*width));
    fclose(f);
    return;
}

// Testing functions ----------------------------------------------------------

// Create a test BMP ----------------------------------------------------------

// Create Bitmap File Header
void createBitmapFileHeader(unsigned char *BitmapHeader, int height, int width, int filesize){
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
void createBitmapInfoHeader(unsigned char *BitmapInfoHeader, int height, int width, int datasize){
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
    int i, j;
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            image[i][j].r = (unsigned char)((double)i/height*255);             // red
            image[i][j].g = (unsigned char)((double)j/width*255);              // green
            image[i][j].b = (unsigned char)(((double)i+j)/(height+width)*255); // blue
        }
    }

    FILE *f = fopen ("image.bmp", "wb");
    int padSize  = (4 - (width * 3) % 4) % 4;
    int filesize = width * height * 3 + 54;
    int datasize = width * height * 3 + height * padSize;

    unsigned char file[14];
    createBitmapFileHeader(file, height, width, filesize);
    unsigned char info[40];
    createBitmapInfoHeader(info, height, width, datasize);

    for(int i = 0; i < 14; i++)
        fputc(file[i], f);
    for(int i = 0; i < 40; i++)
        fputc(info[i], f);

    for(int i = height - 1; i >= 0; i--){
        for(int j = 0; j < width; j++){
            fputc(image[i][j].b, f);
            fputc(image[i][j].g, f);
            fputc(image[i][j].r, f);
        }
    }
    fclose(f);
}

// Test the Huffman encoding
void test(){
    pixel image[541][800];
    int h, w;
    generateBMP(541, 800);
    openBMP("image.bmp", image, &h, &w);
}

// Main function --------------------------------------------------------------
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) {

    }
    else printf("Use:   ./huff   or   ./huff (file)\n");
    return 0;
}
