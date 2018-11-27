#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Testing functions ----------------------------------------------------------

// Create a test BMP ----------------------------------------------------------

// Create Bitmap File Header
void createBitmapFileHeader(unsigned char *BitmapHeader, int height, int width, int filesize){
    for(int i = 0; i < 14; i++)
        BitmapHeader[i] = (unsigned char) (0);

    BitmapHeader[0] = (unsigned char) 'B';
    BitmapHeader[1] = (unsigned char) 'M';
    BitmapHeader[2] = (unsigned char) (filesize);
    BitmapHeader[3] = (unsigned char) (filesize >> 8);
    BitmapHeader[4] = (unsigned char) (filesize >> 16);
    BitmapHeader[5] = (unsigned char) (filesize >> 24);
    BitmapHeader[10] = (unsigned char) (54);

    return;
}

// Create Bitmap Info Header
void createBitmapInfoHeader(unsigned char *BitmapInfoHeader, int height, int width){
    for (int i = 0; i < 40; i++)
        BitmapInfoHeader[i] = (unsigned char)(0);

    BitmapInfoHeader[0] = (unsigned char)(40);
    BitmapInfoHeader[4] = (unsigned char)(width);
    BitmapInfoHeader[5] = (unsigned char)(width >> 8);
    BitmapInfoHeader[6] = (unsigned char)(width >> 16);
    BitmapInfoHeader[7] = (unsigned char)(width >> 24);
    BitmapInfoHeader[8] = (unsigned char)(height);
    BitmapInfoHeader[9] = (unsigned char)(height >> 8);
    BitmapInfoHeader[10] = (unsigned char)(height >> 16);
    BitmapInfoHeader[11] = (unsigned char)(height >> 24);
    BitmapInfoHeader[12] = (unsigned char)(1);
    BitmapInfoHeader[13] = (unsigned char)(24);

    return;
}


void generateBMP(int height, int width){
    unsigned char image[height][width][3];
    int i, j;
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            image[i][j][2] = (unsigned char)((double)i/height*255);             // red
            image[i][j][1] = (unsigned char)((double)j/width*255);              // green
            image[i][j][0] = (unsigned char)(((double)i+j)/(height+width)*255); // blue
        }
    }

    FILE *f = fopen ("image.bmp", "wb");
    int padSize  = (4 - (width * 3) % 4) % 4;
    unsigned char padding[3] = {0, 0, 0};

    int filesize = width * height * 3 + 54;
    int datasize = width * height * 3 + height * padSize;

    // unsigned char BitmapHeader[14];
    // createBitmapFileHeader(BitmapHeader, height, width, filesize);
    // unsigned char BitmapInfo[40];
    // createBitmapInfoHeader(BitmapInfo, height, width);

    unsigned char file[14] = {
        'B','M', // magic
        0,0,0,0, // size in bytes
        0,0, // app data
        0,0, // app data
        40+14,0,0,0 // start of data offset
    };
    unsigned char info[40] = {
        40,0,0,0, // info hd size
        0,0,0,0, // width
        0,0,0,0, // heigth
        1,0, // number color planes
        24,0, // bits per pixel
        0,0,0,0, // compression is none
        0,0,0,0, // image bits size
        0x13,0x0B,0,0, // horz resoluition in pixel / m
        0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
        0,0,0,0, // #colors in pallete
        0,0,0,0, // #important colors
    };

    file[ 2] = (unsigned char)( filesize);
    file[ 3] = (unsigned char)( filesize >> 8);
    file[ 4] = (unsigned char)( filesize >> 16);
    file[ 5] = (unsigned char)( filesize >> 24);

    info[ 4] = (unsigned char)( width   );
    info[ 5] = (unsigned char)( width >> 8);
    info[ 6] = (unsigned char)( width >> 16);
    info[ 7] = (unsigned char)( width >> 24);

    info[ 8] = (unsigned char)( height    );
    info[ 9] = (unsigned char)( height >> 8);
    info[10] = (unsigned char)( height >>16);
    info[11] = (unsigned char)( height >>24);

    info[20] = (unsigned char)( datasize    );
    info[21] = (unsigned char)( datasize >> 8);
    info[22] = (unsigned char)( datasize >>16);
    info[23] = (unsigned char)( datasize >>24);

    for(int i = 0; i < 14; i++)
        fputc(file[i], f);
    for(int i = 0; i < 40; i++)
        fputc(info[i], f);

    for(int i = height - 1; i >= 0; i--){
        for(int j = 0; j < width; j++){
            fputc(image[i][j][0], f);
            fputc(image[i][j][1], f);
            fputc(image[i][j][2], f);
        }
    }

    fclose(f);
}

// Test the Huffman encoding
void test(){
    generateBMP(541, 800);
}

// Main function --------------------------------------------------------------
int main(int n, char *args[n]) {
    if (n == 1) test();
    else if (n == 2) {

    }
    else printf("Use:   ./huff   or   ./huff (file)\n");
    return 0;
}
