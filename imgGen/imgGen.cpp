/** --------------------------------------------------------
* @file    imgGen.cpp
* @author  Alexander Denney
* @version 2.2
*
* CS162-01 - Assignment 1.2
*   Generates an image file in PPM format using 2d arrays and structs
*
* ------------------------------------------------------------ */

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int HEIGHT = 400;
const int WIDTH = 600;
const string FILE_NAME = "orangeDesign.ppm";

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

/** ----------------------------------------------------------
* write given array to disk
*
*   @param image - the array to be written to disk
*   @param height - the height of the image array
*   @param fileName - the file the array will be written to
*   @return false if the file fails to open, true if is successful
*/
bool writeImage(const Color image[][WIDTH], int height, const string fileName) {
    ofstream imageFile;
    imageFile.open(fileName);

    if (!imageFile) {
        cout << "The " << FILE_NAME << " file could not be opened.";
        return false;
    }

    imageFile << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n"; //headers
    
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            Color pixel = image[row][col];
            unsigned char value = '0';
                int red = static_cast<int>(pixel.red);
                int green = static_cast<int>(pixel.green);
                int blue = static_cast<int>(pixel.blue);

                imageFile << red << ' ' << green << ' ' << blue << '\n';
            }
            imageFile << '\n';
    }

    imageFile.close();
    return true;
}


/** ----------------------------------------------------------
* draws a rectangle on the given image (2d array) with given characteristics
*
*   @param img - the array to be written to disk
*   @param imgHeight - the height of the image array
*   @param rectTop -  the y-coordinate of the top left vertex of the rectangle
*   @param rectLeft - the x-coordinate of the top left vertex of the rectangle
*   @param rectHeight - the height of the rectangle
*   @param rectWidth - the width of the rectangle
*   @param color - the color the rectangle will be
*/
void drawRect(Color image[][WIDTH], int imgHeight,
    int rectTop, int rectLeft, int rectHeight, int rectWidth, const Color color) {

    if (rectTop < 0 || rectTop > HEIGHT || rectLeft < 0 || rectLeft > WIDTH || rectLeft + rectWidth > WIDTH || rectTop + rectHeight > HEIGHT) { //bounds checking
        cout << "A rectangle is not being drawn because it is out of bounds!\n";
        return;
    }

    for (int row = 0; row < rectHeight; row++) {
        for (int col = 0; col < rectWidth; col++) {
            image[rectTop + row][rectLeft + col] = color;
        }
    }
    cout << "Drew a rectangle."<< endl;
}

/** ----------------------------------------------------------
* draws a circle on the given image (2d array) with given characteristics
*
*   @param image - the array of the image
*   @param height - the height of the image array
*   @param centerX - the x-coordinate of the center of the circle
*   @param centerY - the y-coordinate of the center of the circle
*   @param radius - the radius of the circle
*   @param color - the color the circle will be
*/
void drawCircle(Color image[][WIDTH], int height,
    int centerX, int centerY, int radius, const Color color) {
    if (centerX > WIDTH || centerX < 0 || centerY > HEIGHT || centerY < 0 || centerY + radius > HEIGHT || centerY - radius < 0 || centerX + radius > WIDTH || centerX - radius < 0) { //bounds checking
        cout << "A circle is not being drawn because it is out of bounds!\n";
        return;
    }

    int negRadius = radius * -1;
    int radiusSquared = radius * radius;
    for (int x = negRadius; x <= radius; x++) {
        int val = radiusSquared - (x * x); //value within square root
        int posY = (int) sqrt(val);
        int negY = posY * -1;

        for (int row = posY + centerY; row >= centerY - posY; row--) { //column-order (slower)
            image[row][x + centerX] = color;
        }
    }
    cout << "Drew a circle." << endl;
}

/** ----------------------------------------------------------
* draws an x-major ellipse on the given image (2d array) with given characteristics and specified gradient colors
*
*   @param image - the array of the image
*   @param height - the height of the image array
*   @param centerX - the x-coordinate of the center of the ellipse
*   @param centerY - the y-coordinate of the center of the ellipse
*   @param majorAxis - the longer diameter of the ellipse - Must be true: majorAxis >= minorAxis
*   @param minorAxis - the shorter diameter of the ellipse 
*   @param color1 - the left (initial) color of the gradient
*   @param color2 - the right (final) color of the gradient
*/
void drawGradientEllipse(Color image[][WIDTH], int height, int centerX, int centerY, int majorAxis, int minorAxis, const Color color1, const Color color2) {   
    if (centerX > WIDTH || centerX < 0 || centerY > HEIGHT || centerY < 0 || centerY + minorAxis/2 > HEIGHT || centerY - minorAxis/2 < 0 || centerX + majorAxis/2 > WIDTH || centerX - majorAxis/2 < 0) { //bounds checking
        cout << "An ellipse is not being drawn because it is out of bounds!\n";
        return;
    }
    
    if (minorAxis > majorAxis) {
        cout << "An ellipse was not drawn because the minorAxis is greater than the majorAxis!" << endl;
        return;
    }
    
    int color1Red = static_cast<int>(color1.red);
    int color2Red = static_cast<int>(color2.red);   
    int color1Green = static_cast<int>(color1.green);
    int color2Green = static_cast<int>(color2.green);  
    int color1Blue = static_cast<int>(color1.blue);
    int color2Blue = static_cast<int>(color2.blue);
    const int redDifference = color2Red - color1Red;
    const int greenDifference = color2Green - color1Green;
    const int blueDifference = color2Blue - color1Blue;

    int negMinorAxis = minorAxis * -1;
    float iterations = 0;
    for (int x = negMinorAxis; x <= minorAxis; x++) { 

        float percentCompleteWithLoop = (iterations)/(minorAxis * 2); //the denominator is the total number of iterations
       
        float value = (minorAxis/2 * (sqrt((majorAxis/2 + x) * (majorAxis/2 - x)))) / (majorAxis/2); //value calculated by ellipse function
        int posY = (int) value;
        int negY = posY * -1;

        for (int row = posY + centerY; row >= centerY - posY; row--) { //column-order
            if (row < height && x + centerX < WIDTH && x+centerX >= 0 && row >=0) {
                int red = (int)(percentCompleteWithLoop * redDifference + color1.red);
                int green = (int)(percentCompleteWithLoop * greenDifference + color1.green);
                int blue = (int)(percentCompleteWithLoop * blueDifference + color1.blue);
                Color intermediateColor = { char(red), char(green), char(blue)};
                image[row][x + centerX] = intermediateColor;
            }
        }
        iterations++;
    }
    cout << "Drew an ellipse." << endl;
}

/** ----------------------------------------------------------
* creates a color image in a 2D array
*
*   @param image - the image array declared in main
*   @param height - the height of the image array
*/
void createImage(Color image[][WIDTH], int height) {
    unsigned int rectTop;
    unsigned int rectLeft;
    unsigned int rectHeight;
    unsigned int rectWidth;
    unsigned int centerX;
    unsigned int centerY;
    unsigned int radius;
    unsigned int minorAxis;
    unsigned int majorAxis;

    const Color BLACK = { 0,0,0 };
    const Color WHITE = { 255,255,255 };
    const Color RED = { 255,0,0 };

    radius = HEIGHT / 2;
    centerX = WIDTH / 2;
    centerY = HEIGHT / 2;
    Color color0 = { 255,68,25 };
    drawCircle(image, HEIGHT, centerX, centerY, radius, color0);

    Color color1 = {255,115,0};
    Color color2 = { 255,2,4};
    majorAxis = HEIGHT - 30;
    drawGradientEllipse(image, HEIGHT, centerX, centerY, majorAxis, majorAxis, color1, color2);

    centerX = 300;
    centerY = 200;
    majorAxis = WIDTH/2;
    minorAxis = HEIGHT/2;
    Color color3 = { 255,255,0 };
    Color color4 = { 255,120,50 };
    drawGradientEllipse(image, HEIGHT, centerX, centerY, majorAxis, minorAxis, color3, color4);

    Color color5 = { 255,255,100 };
    Color color6 = { 255,250,150 };
    majorAxis = 100;
    drawGradientEllipse(image, HEIGHT, centerX, centerY, majorAxis, majorAxis, color3, color4);

    radius = 40;
    drawCircle(image, HEIGHT, centerX, centerY, radius, WHITE);

    rectTop = 20;
    rectLeft = 0;
    rectHeight = 10;
    rectWidth = WIDTH;
    drawRect(image, HEIGHT, rectTop, rectLeft, rectHeight, rectWidth, RED);

    rectTop = HEIGHT-20;
    drawRect(image, HEIGHT, rectTop, rectLeft, rectHeight, rectWidth, RED);

    cout << "Done drawing image." << endl;
}

//=============================================================
int main() {
    static Color image[HEIGHT][WIDTH];
    createImage(image, HEIGHT);
    writeImage(image, HEIGHT, FILE_NAME);

   return 0;
}