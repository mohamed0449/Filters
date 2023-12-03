// Mohamed Gomaa 20220449 mhmd2004221@gmail.com

#include <bits/stdc++.h>
#define fudou cin.tie(0),cout.tie(0),cin.sync_with_stdio(0),cout.sync_with_stdio(0);
#include <unistd.h>
#include "cmake-build-debug/bmplib.h"
#define E '\n'

unsigned char imgGs[SIZE][SIZE];
using namespace std;

int main() {
    fudou
    string inputname, path = "\\imgs\\";
    cout << "Enter image name : ";
    cout.flush();
    cin >> inputname;
    path += inputname;
    char cwd[PATH_MAX];
    const char *outputFilename = "output.bmp";
    string path2 = "\\imgs\\";
    int userChoice = displayFilterMenu();
    float brightnessFactor = 1.5; // Change this value to adjust brightness
    readGSBMP(strcat(getcwd(cwd, sizeof(cwd)), path.c_str()), imgGs);
    switch (userChoice) {
        case 1:
            //// Apply the Black and White Filter
            BlackAndWhiteFilter(imgGs);
//// Save the filtered image
            writeGSBMP("black_and_white.bmp", imgGs);
            showGSBMP(imgGs);
            break;
        case 2:
            //// Apply the Invert Filter
            InvertFilter(imgGs);
//// Save the inverted image
            writeGSBMP("inverted.bmp", imgGs);
            showGSBMP(imgGs);
            break;
        case 3:
            cout << "Enter the name of the second image: ";
            cout.flush();
            cin >> inputname;
            path2 += inputname;
            // Load the second image
            unsigned char img2[SIZE][SIZE];
            readGSBMP(strcat(getcwd(cwd, sizeof(cwd)), path2.c_str()), img2);
            // Create the output image array
            unsigned char mergedImage[SIZE][SIZE];
            // Merge the two images
            merge(imgGs, img2, mergedImage);
            // Display or save the merged image
            showGSBMP(mergedImage);
            break;
        case 4:
            // Create a copy for flipping
            unsigned char imgFlipped[SIZE][SIZE];
// Flip the image horizontally
            flipHorizontal(imgGs, imgFlipped);
// Show the horizontally flipped image
            showGSBMP(imgFlipped);
// Flip the image vertically
            flipVertical(imgGs, imgFlipped);
// Show the vertically flipped image
            showGSBMP(imgFlipped);
            break;
        case 5:
            // Create copies for rotation
            unsigned char imgRotated90[SIZE][SIZE];
            unsigned char imgRotated180[SIZE][SIZE];
            unsigned char imgRotated270[SIZE][SIZE];
            cout << "which degree do you want? (choose 90 or 180 or 270)" << E;
            cout.flush();
            int degree;
            cin >> degree;
            // Rotate the image by 90 degrees and display
            if (degree == 90) {
                rotate90(imgGs, imgRotated90);
                showGSBMP(imgRotated90);
            }
//    // Rotate the image by 180 degrees and display
            else if (degree == 180) {
                rotate180(imgGs, imgRotated180);
                showGSBMP(imgRotated180);
            }
//    // Rotate the image by 270 degrees and display
            else if (degree == 270) {
                rotate270(imgGs, imgRotated270);
                showGSBMP(imgRotated270);
            } else {
                cout << "does not exist" << E;
                cout.flush();
            }
            break;
        case 6:
//            Create a copy for adjusting brightness
            unsigned char imgAdjustedBrightness[SIZE][SIZE];
            // Adjust the brightness of the image by 50% (0.5 for darker, 1.5 for lighter)
            DarkenandLightenImage(imgGs, imgAdjustedBrightness, brightnessFactor);
            // Show the adjusted brightness image
            showGSBMP(imgAdjustedBrightness);

            break;
        case 7:
            //            Apply edge detection filter
            unsigned char edgeImage[SIZE][SIZE];
            detectEdges(imgGs, edgeImage);
            // Show the edge-detected image
            showGSBMP(edgeImage);
            break;
        case 8:
            unsigned char img[SIZE][SIZE];
            unsigned char enlargedImg[SIZE][SIZE];
            // Load your image into 'img' here using readGSBMP or readRGBBMP
            int selectedQuarter;
            cout << "Select a quarter to enlarge (1, 2, 3, or 4): ";
            cout.flush();
            cin >> selectedQuarter;
            // Ensure the selected quarter is valid
            if (selectedQuarter >= 1 && selectedQuarter <= 4) {
                enlargeQuarter(imgGs, enlargedImg, selectedQuarter);

                // Save or display the enlarged quarter here using writeGSBMP or writeRGBBMP
                showGSBMP(enlargedImg);
            } else {
                cout << "Invalid quarter selection." << endl;
                cout.flush();
            }
            break;
        case 9:
            unsigned char shrunkImg[SIZE][SIZE];
            int scaleFactor;
            cout << "Enter the scale factor (2, 3, or 4): ";
            cout.flush();
            cin >> scaleFactor;
            // Ensure that the scaleFactor is valid
            if (scaleFactor != 2 && scaleFactor != 3 && scaleFactor != 4) {
                cout << "Invalid scale factor. Please enter 2, 3, or 4." << endl;
                cout.flush();
                return 1; // Exit with an error code
            }
//    // Call the shrinkImage function
            shrinkImage(imgGs, shrunkImg, scaleFactor);
//    // Display or save the shrunk image
            showGSBMP(shrunkImg);
            break;
        case 10:
            int choice;
            cout << "Choose a mirror filter (1: Left 1/2, 2: Right 1/2, 3: Upper 1/2, 4: Lower 1/2): ";
            cout.flush();
            cin >> choice;
            switch (choice) {
                case 1:
                    mirrorLeftHalf(imgGs);
                    break;
                case 2:
                    mirrorRightHalf(imgGs);
                    break;
                case 3:
                    mirrorUpperHalf(imgGs);
                    break;
                case 4:
                    mirrorLowerHalf(imgGs);
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    cout.flush();
                    return 1;
            }
            showGSBMP(imgGs);
            break;
        case 11:
            unsigned char shuffledImage[SIZE][SIZE];
            int order[4];
//    // Prompt the user to enter the order for shuffling
            cout << "Enter the order for shuffling (e.g., 1 2 3 4): ";
            cout.flush();
            for (int i = 0; i < 4; i++) {
                cin >> order[i];
            }
//    // Shuffle the quarters based on user-defined order
            shuffleQuarters(imgGs, shuffledImage, order);
//    // Display the shuffled image using showGSBMP
            showGSBMP(shuffledImage);
            break;
        case 12:
            unsigned char blurredImage[SIZE][SIZE];
//    // Apply the blur filter to the image
            blurImage(imgGs, blurredImage);
//    // Now you can display or save the blurred image as needed
//    // For displaying using your image library's function, refer to its documentation
//    // For example, to display using your library's function:
            showGSBMP(blurredImage);
            break;
        case 13:
            int x, y, l, w;
//
            cout << "Enter the x-coordinate of the top-left corner of the square: ";
            cout.flush();
            cin >> x;
            cout << "Enter the y-coordinate of the top-left corner of the square: ";
            cout.flush();
            cin >> y;
            cout << "Enter the length of the square: ";
            cout.flush();
            cin >> l;
            cout << "Enter the width of the square: ";
            cout.flush();
            cin >> w;
//    // Create an output image for the cropped result
            unsigned char croppedImage[SIZE][SIZE];
//    // Apply the crop filter to the image
            cropImage(imgGs, croppedImage, x, y, l, w);
//    // Now you can display or save the cropped image as needed
//    // For displaying using your image library's function, refer to its documentation
//    // For example, to display using your library's function:
            showGSBMP(croppedImage);
//    // Optionally, you can save the cropped image using your library's function
            writeGSBMP(outputFilename, croppedImage);
            break;
        case 14:
            unsigned char inputImage[IMAGE_SIZE][IMAGE_SIZE];
            // Initialize outputImage to store the skewed image
            unsigned char outputImage[IMAGE_SIZE][IMAGE_SIZE];
            // Degree of skewing, you can adjust this value
            float skewDegree;
            //Choose to skew horizontally (e) or vertically (f)
            char choic;
            cout << "Choose skew direction (e for horizontal, f for vertical): ";
            cout.flush();
            cin >> choic;
            // Check if the user's choice is valid
            if (choic != 'e' && choic != 'f') {
                cout << "Invalid choice. Please select 'e' for horizontal or 'f' for vertical skewing." << endl;
                cout.flush();
                return 1;
            }
            // Ask the user for the degree of skewing
            cout << "Enter the degree of skewing (positive for right/up, negative for left/down): ";
            cout.flush();
            cin >> skewDegree;
            // Apply the chosen skewing
            skewImage(inputImage, outputImage, skewDegree, choic);
            // Display or save the skewed image using showGSBMP or writeGSBMP (assuming you have these functions)
            showGSBMP(outputImage);
            break;
        default:
            cout << "Invalid choice. Please select a valid filter (1-14)." << E;
            cout.flush();
    }
}