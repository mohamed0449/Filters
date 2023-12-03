#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "bmplib.h"
#include <unistd.h>
#define E '\n'

using namespace std;
using std::cout;
using std::cin;
using std::endl;

typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

//#define BMP_BIG_ENDIAN
#define BYTE_SWAP(num) (((num>>24)&0xff) | ((num<<8)&&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

typedef struct {
    uint8    bfType1;
    uint8    bfType2;
    uint32   bfSize;
    uint16    bfReserved1;
    uint16    bfReserved2;
    uint32   bfOffBits;
    uint32   biSize;          // size of structure, in bytes
    uint32   biWidth;         // bitmap width, in pixels
    uint32   biHeight;        // bitmap height, in pixels
    uint16    biPlanes;        // see below
    uint16    biBitCount;      // see below
    uint32   biCompression;   // see below
    uint32   biSizeImage;     // see below
    uint32   biXPelsPerMeter; // see below
    uint32   biYPelsPerMeter; // see below
    uint32   biClrUsed;       // see below
    uint32   biClrImportant;  // see below
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct {
    uint8 rgbBlue;
    uint8 rgbGreen;
    uint8 rgbRed;
} RGBTRIPLE;
void write_hdr(uint8 *hdr, int *hdr_idx, uint32 data, uint32 size);
uint8 tempImage[SIZE][SIZE][RGB];
uint8 tempGSImage[SIZE][SIZE];
int readRGBBMP(const char* filename, unsigned char inputImage[][SIZE][RGB])
{
    uint8 type[2];
    int headersize = 0;
    int i,j,k;
    //BITMAPFILEHEADER bfh;
    /* file pointer */
    FILE *file;
    /* read input bmp into the data matrix */
    if (!(file=fopen(filename,"rb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        return(1);
    }
    fread(type, sizeof(unsigned char), 0x2, file);
    if(type[0] != 'B' and type[1] != 'M'){
        cout << "Not a BMP file" << endl;
        return(1);
    }
    fseek(file, 8, SEEK_CUR);
    fread(&headersize, sizeof(uint32), 1, file);
#ifdef BMP_BIG_ENDIAN
    headersize = BYTE_SWAP(headersize);
#endif
    //cout << "Header size is " << headersize << endl;
    fseek(file, headersize, SEEK_SET);
    fread(tempImage, sizeof(uint8), SIZE*SIZE*RGB, file);
    // cout << (int)tempImage[0][0][0] << "," << (int)tempImage[0][0][1] << "," << (int)tempImage[0][0][2] << endl;
    fclose(file);
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            for(k=0; k < RGB; k++){
                inputImage[SIZE-1-i][j][RGB-1-k] = tempImage[i][j][k];
                //cerr << tempImage[i][j][k] << ",";
            }
        }
        //cerr << endl;
    }
    return 0;
}

int writeRGBBMP(const char* filename, unsigned char outputImage[][SIZE][RGB])
{
    uint8 hdr[54];
    int hdr_idx = 0;
    int i,j,k;
    BITMAPFILEHEADER bfh;
    // file pointer
    FILE *file;
    bfh.bfType1 = 'B';
    bfh.bfType2 = 'M';
    bfh.bfSize = 0x36;
    bfh.bfReserved1 = 0x0;
    bfh.bfReserved2 = 0x0;
    bfh.bfOffBits = 0x36;
    bfh.biSize = 0x28;
    bfh.biWidth = SIZE;
    bfh.biHeight = SIZE;
    bfh.biPlanes = 1;
    bfh.biBitCount = 24;
    bfh.biCompression  = 0;
    bfh.biSizeImage = sizeof(RGBTRIPLE)*SIZE*SIZE;
    bfh.biXPelsPerMeter = 2400;
    bfh.biYPelsPerMeter = 2400;
    bfh.biClrUsed = 0;
    bfh.biClrImportant = 0;
    write_hdr(hdr, &hdr_idx, bfh.bfType1, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfType2, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved1, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved2, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfOffBits, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biWidth, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biHeight, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biPlanes, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biBitCount, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biCompression, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSizeImage, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biXPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biYPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrUsed, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrImportant, sizeof(uint32));
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            for(k=0; k < RGB; k++){
                tempImage[SIZE-1-i][j][RGB-1-k] = outputImage[i][j][k];
            }
        }
    }
    // write result bmp file
    if (!(file=fopen(filename,"wb")))
    {
        cout << "Cannot open file: " << filename << endl;
        return(1);
    }
    fwrite(&hdr, sizeof(unsigned char), 0x36, file);
    fwrite(tempImage, sizeof(unsigned char), SIZE*SIZE*RGB, file);
    fclose(file);

    return 0;
}

void write_hdr(uint8 *hdr, int *hdr_idx, uint32 data, uint32 size)
{
    if(size == 1){
        hdr[*hdr_idx] = (uint8) data;
        (*hdr_idx) += 1;
    }
    else if(size == 2){
        hdr[*hdr_idx] = (uint8) (data & 0x00ff);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0xff00) >> 8);
        (*hdr_idx) += 1;
    }
    else if(size == 4){
        hdr[*hdr_idx] = (uint8) (data & 0x000000ff);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0x0000ff00) >> 8);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0x00ff0000) >> 16);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0xff000000) >> 24);
        (*hdr_idx) += 1;
    }
    else {
        printf("Illegal size in write_hdr...consult your instructor\n");
    }
}

int readGSBMP(const char* filename, unsigned char inputImage[][SIZE])
{
    uint8 type[2];
    int headersize = 0;
    int i,j;
    //BITMAPFILEHEADER bfh;
    /* file pointer */
    FILE *file;
    /* read input bmp into the data matrix */
    if (!(file=fopen(filename,"rb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        return(1);
    }
    fread(type, sizeof(unsigned char), 0x2, file);
    if(type[0] != 'B' and type[1] != 'M'){
        cout << "Not a BMP file" << endl;
        return(1);
    }
    fseek(file, 8, SEEK_CUR);
    fread(&headersize, sizeof(uint32), 1, file);
#ifdef BMP_BIG_ENDIAN
    headersize = BYTE_SWAP(headersize);
#endif
    //cout << "Header size is " << headersize << endl;
    fseek(file, headersize, SEEK_SET);
    fread(tempGSImage, sizeof(uint8), SIZE*SIZE, file);
    //  cout << (int)tempGSImage[0][0][0] << "," << (int)tempGSImage[0][0][1] << "," << (int)tempGSImage[0][0][2] << endl;
    fclose(file);

    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            inputImage[SIZE-1-i][j] = tempGSImage[i][j];
            //	inputImage[SIZE-1-i][SIZE-1-j] = tempGSImage[i][j];
            //cerr << tempGSImage[i][j][k] << ",";
        }
        //cerr << endl;
    }
    return 0;
}

int writeGSBMP(const char* filename, unsigned char outputImage[][SIZE])
{
    uint8 hdr[54];
    int hdr_idx = 0;
    int i,j;
    BITMAPFILEHEADER bfh;
    // file pointer
    FILE *file;
    bfh.bfType1 = 'B';
    bfh.bfType2 = 'M';
    // 0x10436 = 2^16 + 1024 for color def + 0x36 for header
    bfh.bfSize = 0x010436; //0x36;
    bfh.bfReserved1 = 0x0;
    bfh.bfReserved2 = 0x0;
    // 0x0436
    bfh.bfOffBits = 0x436; //0x36;
    bfh.biSize = 0x28;
    bfh.biWidth = SIZE;
    bfh.biHeight = SIZE;
    bfh.biPlanes = 1;
    bfh.biBitCount = 8;
    bfh.biCompression  = 0;
    bfh.biSizeImage = SIZE*SIZE;
    bfh.biXPelsPerMeter = 0; //2400;
    bfh.biYPelsPerMeter = 0; //2400;
    bfh.biClrUsed = SIZE; // 0;
    bfh.biClrImportant = SIZE; // 0;
    write_hdr(hdr, &hdr_idx, bfh.bfType1, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfType2, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved1, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved2, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfOffBits, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biWidth, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biHeight, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biPlanes, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biBitCount, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biCompression, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSizeImage, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biXPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biYPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrUsed, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrImportant, sizeof(uint32));


    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            //tempGSImage[SIZE-1-i][SIZE-1-j] = outputImage[i][j];
            tempGSImage[SIZE-1-i][j] = outputImage[i][j];
        }
    }

    // write result bmp file
    if (!(file=fopen(filename,"wb")))
    {
        cout << "Cannot open file: " << filename << endl;
        return(1);
    }
    uint8 z = 0;
    fwrite(&hdr, sizeof(unsigned char), 0x36, file);
    for(i=0; i < SIZE; i++){
        uint8 x = (uint8) i;
        //cout << (int)x << endl;
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&z, sizeof(uint8), 0x01, file);

    }
    fwrite(tempGSImage, sizeof(unsigned char), SIZE*SIZE, file);
    fclose(file);


    return 0;
}

int shows = 0;
void show() {
    string command = "\\tmp\\bmplib.bmp";
    char cwd[PATH_MAX];
    system(strcat(getcwd(cwd, sizeof(cwd)), command.c_str()));
    usleep(200000);
    shows++;
}

void showRGBBMP(unsigned char inputImage[][SIZE][RGB]) {
    writeRGBBMP("./tmp/bmplib.bmp", inputImage);
    show();
}

void showGSBMP(unsigned char inputImage[][SIZE]) {
    writeGSBMP("./tmp/bmplib.bmp", inputImage);
    show();
}

//This is our functions:
void BlackAndWhiteFilter(unsigned char inputImage[][SIZE]) {
    int sum = 0;
    int numPixels = SIZE * SIZE;
    // Calculate the average gray level
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum += inputImage[i][j];
        }
    }
    int averageGrayLevel = sum / numPixels;
    // Threshold the image
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            inputImage[i][j] = (inputImage[i][j] > averageGrayLevel) ? 255 : 0;
        }
    }
}
void InvertFilter(unsigned char (*inputImage)[256]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            // Calculate the inverse pixel value
            inputImage[i][j] = 255 - inputImage[i][j];
        }
    }
}

void flipHorizontal(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImage[i][j] = inputImage[i][width - 1 - j];
        }
    }
}

void flipVertical(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImage[i][j] = inputImage[height - 1 - i][j];
        }
    }
}

void rotate90(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImage[j][height - 1 - i] = inputImage[i][j];
        }
    }
}

void rotate180(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImage[height - 1 - i][width - 1 - j] = inputImage[i][j];
        }
    }
}

void rotate270(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outputImage[width - 1 - j][i] = inputImage[i][j];
        }
    }
}

void detectEdges(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256]) {
    int height = SIZE;
    int width = SIZE;
    // Apply the Sobel operator for edge detection
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int gx = -inputImage[i - 1][j - 1] + inputImage[i - 1][j + 1] -
                     2 * inputImage[i][j - 1] + 2 * inputImage[i][j + 1] -
                     inputImage[i + 1][j - 1] + inputImage[i + 1][j + 1];
            int gy = -inputImage[i - 1][j - 1] - 2 * inputImage[i - 1][j] - inputImage[i - 1][j + 1] +
                     inputImage[i + 1][j - 1] + 2 * inputImage[i + 1][j] + inputImage[i + 1][j + 1];
            int gradient = abs(gx) + abs(gy);
            // Threshold the gradient to detect edges
            if (gradient > 100) {
                outputImage[i][j] = 255; // Edge pixel
            } else {
                outputImage[i][j] = 0; // Non-edge pixel
            }
        }
    }
}

void enlargeQuarter(const unsigned char (*inputImage)[256], unsigned char (*outputImage)[256], int quarter) {
    int startX = 0, startY = 0;
    if (quarter == 1) {
        startX = 0;
        startY = 0;
    } else if (quarter == 2) {
        startX = SIZE / 2;
        startY = 0;
    } else if (quarter == 3) {
        startX = 0;
        startY = SIZE / 2;
    } else if (quarter == 4) {
        startX = SIZE / 2;
        startY = SIZE / 2;
    } else {
        std::cout << "Invalid quarter selection." << std::endl;
        return;
    }
    for (int i = 0; i < SIZE / 2; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            unsigned char pixel = inputImage[startY + i][startX + j];
            outputImage[i * 2][j * 2] = pixel;
            outputImage[i * 2 + 1][j * 2] = pixel;
            outputImage[i * 2][j * 2 + 1] = pixel;
            outputImage[i * 2 + 1][j * 2 + 1] = pixel;
        }
    }
}

void shrinkImage(const unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE], int scaleFactor) {
    int shrunkHeight = SIZE / scaleFactor;
    int shrunkWidth = SIZE / scaleFactor;
    for (int i = 0; i < shrunkHeight; i++) {
        for (int j = 0; j < shrunkWidth; j++) {
            // Calculate the average pixel value in a block based on the scaleFactor
            int sum = 0;
            for (int x = 0; x < scaleFactor; x++) {
                for (int y = 0; y < scaleFactor; y++) {
                    sum += inputImage[i * scaleFactor + x][j * scaleFactor + y];
                }
            }
            outputImage[i][j] = static_cast<unsigned char>(sum / (scaleFactor * scaleFactor));
        }
    }
}



void mirrorLeftHalf(unsigned char img[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            img[i][j] = img[i][SIZE - 1 - j];
        }
    }
}

void mirrorRightHalf(unsigned char img[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = SIZE / 2; j < SIZE ; j++) {
            img[i][j] = img[i][SIZE - 1 - j];
        }
    }
}

void mirrorUpperHalf(unsigned char img[][SIZE]) {
    int halfHeight = SIZE / 2;
    for (int i = 0; i < halfHeight; i++) {
        for (int j = 0; j < SIZE; j++) {
            img[i][j] = img[SIZE - 1 - i][j];
        }
    }
}

void mirrorLowerHalf(unsigned char img[][SIZE]) {
    int halfHeight = SIZE / 2;
    for (int i = halfHeight; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            img[i][j] = img[SIZE - 1 - i][j];
        }
    }
}

void shuffleQuarters(const unsigned char (*inputImage)[SIZE], unsigned char (*outputImage)[SIZE], const int order[4]) {
    // Check for valid input order (1 to 4, no duplicates)
    bool validOrder = true;
    bool seen[4] = {false};
    for (int i = 0; i < 4; i++) {
        if (order[i] < 1 || order[i] > 4 || seen[order[i] - 1]) {
            validOrder = false;
            break;
        }
        seen[order[i] - 1] = true;
    }

    if (!validOrder) {
        cout << "Error: Invalid order. Please provide a valid order (1 to 4 with no duplicates)." << endl;
        return;
    }
    // Calculate the dimensions of each quarter
    int quarterWidth = SIZE / 2;
    int quarterHeight = SIZE / 2;
    // Copy quarters based on the user's input order
    for (int i = 0; i < 4; i++) {
        int srcRowStart, srcColStart, destRowStart, destColStart;
        switch (order[i]) {
            case 1:
                srcRowStart = 0;
                srcColStart = 0;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 2:
                srcRowStart = 0;
                srcColStart = quarterWidth;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 3:
                srcRowStart = quarterHeight;
                srcColStart = 0;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 4:
                srcRowStart = quarterHeight;
                srcColStart = quarterWidth;
                destRowStart = 0;
                destColStart = 0;
                break;
            default:
                break;
        }
        // Copy the quarter to the output image
        for (int row = 0; row < quarterHeight; row++) {
            for (int col = 0; col < quarterWidth; col++) {
                outputImage[destRowStart + row][destColStart + col] = inputImage[srcRowStart + row][srcColStart + col];
            }
        }
    }
}

void blurImage(unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE]) {
    // Define a Gaussian kernel for blurring
    float kernel[3][3] = {
            {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
            {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
            {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
    };
    // Iterate over each pixel in the image
    for (int y = 1; y < SIZE - 1; y++) {
        for (int x = 1; x < SIZE - 1; x++) {
            float sum = 0.0;
            // Apply the Gaussian kernel to the pixel and its neighbors
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    sum += kernel[ky + 1][kx + 1] * inputImage[y + ky][x + kx];
                }
            }
            // Store the blurred pixel value in the output image
            outputImage[y][x] = static_cast<unsigned char>(sum);
        }
    }
}

void cropImage(const unsigned char inputImage[][SIZE], unsigned char outputImage[][SIZE], int x, int y, int l, int w) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < l; j++) {
            outputImage[i][j] = inputImage[y + i][x + j];
        }
    }
}
void skewImage(unsigned char inputImage[IMAGE_SIZE][IMAGE_SIZE], unsigned char outputImage[IMAGE_SIZE][IMAGE_SIZE], float skewDegree, char direction) {
    // Calculate the tangent of the skew angle
    float skewTan = std::tan(skewDegree);

    for (int i = 0; i < IMAGE_SIZE; i++) {
        for (int j = 0; j < IMAGE_SIZE; j++) {
            // Calculate the source position based on skew direction
            int srcX, srcY;

            if (direction == 'e') {  // Horizontal skew (right)
                srcX = i;
                srcY = j - static_cast<int>(skewTan * i);
            } else if (direction == 'f') {  // Vertical skew (up)
                srcX = i - static_cast<int>(skewTan * j);
                srcY = j;
            } else {
                std::cerr << "Invalid direction. Please use 'e' for horizontal or 'f' for vertical skewing." << std::endl;
                return;
            }

            // Ensure the source coordinates are within bounds
            if (srcX >= 0 && srcX < IMAGE_SIZE && srcY >= 0 && srcY < IMAGE_SIZE) {
                // Copy the pixel from the source to the destination
                outputImage[i][j] = inputImage[srcX][srcY];
            } else {
                // If the source position is out of bounds, set the destination pixel to black.
                outputImage[i][j] = 0;  // Set to black
            }
        }
    }
}


void DarkenandLightenImage(unsigned char (*inputImage)[256], unsigned char (*outputImage)[256], float factor) {
    int height = SIZE;
    int width = SIZE;
    // Ensure the factor is between -1.0 and 1.0
    if (factor < -1.0) {
        factor = -1.0;
    } else if (factor > 1.0) {
        factor = 0.5;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Adjust the brightness of each pixel
            int newValue = inputImage[i][j] + factor * 255;
            // Ensure the new value is within the valid range [0, 255]
            if (newValue < 0) {
                newValue = 0;
            } else if (newValue > 255) {
                newValue = 255;
            }
            outputImage[i][j] = static_cast<unsigned char>(newValue);
        }
    }
}

void
merge(unsigned char (*inputImage1)[256], unsigned char (*inputImage2)[256], unsigned char (*outputImage)[256]) {
    // Loop through each pixel in the images
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Calculate the average gray level
            outputImage[i][j] = (inputImage1[i][j] + inputImage2[i][j]) / 2;
        }
    }
}

int displayFilterMenu(){
    int choose;
    cout << "Choose from these filters:" <<E;
    cout << "1. Filter 1: Black and White Image" << E;
    cout << "2. Filter 2: Invert Image" << E;
    cout << "3. Filter 3: Merge Images" << E;
    cout << "4. Filter 4: Flip" << E;
    cout << "5. Filter 5: Rotate Image" << E;
    cout << "6. Filter 6: Darken and Lighten Image" << E;
    cout << "7. Filter 7: Detect Image Edges" << E;
    cout << "8. Filter 8: Enlarge Image" << E;
    cout << "9. Filter 9: Shrink Image" << E;
    cout << "10. Filter 10: Mirror Image" << E;
    cout << "11. Filter 11: Shuffle Image" << E;
    cout << "12. Filter 12: Blur Image" << E;
    cout << "13. Filter 13: Crop Image" << E;
    cout << "14. Filter 14: Skew Horizontally / Vertically" << E;
    cout << "Enter your choice (1-14): "<<E;
    cout.flush();
    cin >> choose;
    return choose;
}