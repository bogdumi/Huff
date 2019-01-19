# Huff

“huff.c” implements a Huffman encoding algorithm for a BMP image. Testing required implementing a functionality that generates a new BMP image, so I decided to make this a secondary feature of the program.

## Install

A Makefile is included in the repo with the necessary flags, and it will work on Windows, MacOS and any Linux distro, so just type `make` in the folder with the `huff.c` source file.

## Running

The program can be run with:
- `./huff` for testing (also generates a 100x100 BMP image)
- `./huff <filename>` for encoding an image, outputs the result to `output.txt`.
- `./huff generate <height> <width>` for generating an image with a given height and width with the filename `image.bmp`. 

## Extra info

Each pixel in the image generated during testing has a unique RGB value, so the file that the Huffman encoding algorithm has a disproportionately large size (the 33KB 100x100 test image outputs a 5.3MB file), because each pixel requires a different node. However, if the image has fewer unique pixels, the filesize is greatly reduced (a 33KB 100x100 test image with 1 colour outputs a 16B file, the RGB values of the pixel).

The only limitation this program has is with very high values for height\*width, which can cause a stack overflow. I've limited the product at 1670\*1670 for generating an image (so that it can still generate a 1080x1920 image, for a 1080p wallpaper, for example) and at 600\*600 for the Huffman encoding itself.

## Resources used:

Bitmap image information: https://web.archive.org/web/20080912171714/http://www.fortunecity.com/skyscraper/windows/364/bmpffrmt.html

Bitmap image processing: https://en.wikipedia.org/wiki/Bitmap, https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries

Huffman algorithm: https://en.wikipedia.org/wiki/Huffman_coding, https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
