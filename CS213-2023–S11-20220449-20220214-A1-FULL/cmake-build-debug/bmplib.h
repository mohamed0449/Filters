#ifndef BMPLIB_H
#define BMPLIB_H
#include <bits/stdc++.h>

const int SIZE = 256;
const int RGB = 3;
const int IMAGE_SIZE = 256;

// read full-color image from the file specified by filename, into inputImage
int readRGBBMP(const char* filename, unsigned char inputImage[][SIZE][RGB]);

// write full-color image to the file specified by filename, from outputImage
int writeRGBBMP(const char* filename, unsigned char outputImage[][SIZE][RGB]);

// display full-color image with eog, pause 0.2 seconds. (uses a temp file)
void showRGBBMP(unsigned char inputImage[][SIZE][RGB]);

// read grayscale image from the file specified by filename, into inputImage
int readGSBMP(const char* filename, unsigned char image[][SIZE]);

// write grayscale image to the file specified by filename, from outputImage
int writeGSBMP(const char* filename, unsigned char outputImage[][SIZE]);

// display grayscale image with eog, pause 0.2 seconds. (uses a temp file)
void showGSBMP(unsigned char inputImage[][SIZE]);

int displayFilterMenu();
void BlackAndWhiteFilter(unsigned char inputImage[][SIZE]);
void InvertFilter(unsigned char inputImage[][SIZE]);
void merge(unsigned char inputImage1[SIZE][SIZE], unsigned char inputImage2[SIZE][SIZE], unsigned char outputImage[SIZE][SIZE]) ;
void flipHorizontal(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]);
void flipVertical(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]);
void rotate90(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]) ;
void rotate180(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]) ;
void rotate270(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]) ;
void DarkenandLightenImage(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256], float factor);
void detectEdges(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]) ;
void enlargeQuarter(const unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE], int quarter);
void shrinkImage(const unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE], int scaleFactor) ;
void mirrorLeftHalf(unsigned char img[][SIZE]) ;
void mirrorRightHalf(unsigned char img[][SIZE]);
void mirrorUpperHalf(unsigned char img[][SIZE]);
void mirrorLowerHalf(unsigned char img[][SIZE]) ;
void shuffleQuarters(const unsigned char (*inputImage)[SIZE], unsigned char (*outputImage)[SIZE], const int order[4]);
void blurImage(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]);
void cropImage(const unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE], int x, int y, int l, int w) ;
void skewImage(unsigned char inputImage[IMAGE_SIZE][IMAGE_SIZE], unsigned char outputImage[IMAGE_SIZE][IMAGE_SIZE], float skewDegree, char direction);
#endif
