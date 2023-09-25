#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>

// Swap pixels
void swap(int height, int width, RGBTRIPLE image[height][width], int x, int y);

// Calculate blured pixel value
RGBTRIPLE blured_pixel(int height, int width, RGBTRIPLE image[height][width], int x, int y);

// Sobel functions
RGBTRIPLE sobel(int height, int width, RGBTRIPLE image[height][width], int x, int y);
int sobelRed(int height, int width, RGBTRIPLE image[height][width], int x, int y);
int sobelGreen(int height, int width, RGBTRIPLE image[height][width], int x, int y);
int sobelBlue(int height, int width, RGBTRIPLE image[height][width], int x, int y);

// Return the lesser value
int min(int x, int y);

// Return the greater value
int max(int x, int y);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate average RGB value for pixel
            int average_rgb = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Set pixel values to the average
            image[i][j].rgbtRed = average_rgb;
            image[i][j].rgbtGreen = average_rgb;
            image[i][j].rgbtBlue = average_rgb;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        if (width % 2 == 0)
        {
            for (int j = 0; j < width / 2; j++)
            {
                swap(height, width, image, i, j);
            }
        }
        else
        {
            for (int j = 0; j < (width / 2) + 1; j++)
            {
                swap(height, width, image, i, j);
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Load pixels into new 2D array
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, height * width * sizeof(RGBTRIPLE));

    // Blur individual pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blured_pixel(height, width, copy, i, j);
        }
    }
    return;
}

// Calculate blured pixel value
RGBTRIPLE blured_pixel(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    RGBTRIPLE blured_pixel;

    int avgRed = 0;
    int avgGreen = 0;
    int avgBlue = 0;

    float counter = 0;

    // Determain starting and ending cords for loop
    int startPosX = max(x - 1, 0);
    int startPosY = max(y - 1, 0);
    int endPosX = min(x + 1, height - 1);
    int endPosY = min(y + 1, width - 1);

    // Loop trough pixels and add up the RGB values
    for (int i = startPosX; i <= endPosX; i++)
    {
        for (int j = startPosY; j <= endPosY; j++)
        {
            avgRed = avgRed + image[i][j].rgbtRed;
            avgGreen = avgGreen + image[i][j].rgbtGreen;
            avgBlue = avgBlue + image[i][j].rgbtBlue;

            counter++;
        }
    }

    // Calculate the average for each RGB
    avgRed = round(avgRed / counter);
    avgGreen = round(avgGreen / counter);
    avgBlue = round(avgBlue / counter);

    // Set average values to RGBTRIPLE
    blured_pixel.rgbtRed = min(avgRed, 255);
    blured_pixel.rgbtGreen = min(avgGreen, 255);
    blured_pixel.rgbtBlue = min(avgBlue, 255);

    return blured_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create and copy contents into new 2D array to work with
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, height * width * sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = sobel(height, width, copy, i, j);
        }
    }
    return;
}

RGBTRIPLE sobel(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    RGBTRIPLE pixel = image[x][y];

    pixel.rgbtRed = sobelRed(height, width, image, x, y);
    pixel.rgbtGreen = sobelGreen(height, width, image, x, y);
    pixel.rgbtBlue = sobelBlue(height, width, image, x, y);

    return pixel;
}

int sobelRed(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    int rgbtRed = 0;
    int Gx = 0;
    int Gy = 0;

    // Determain starting and ending cords for loop
    int startPosX = x - 1;
    int startPosY = y - 1;
    int endPosX = x + 1;
    int endPosY = y + 1;

    // Initialize array containing order of Sobel operator numbers
    int sobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int sobelPos = 0;

    for (int i = startPosX; i <= endPosX; i++)
    {
        for (int j = startPosY; j <= endPosY; j++)
        {
            // Check if pixel is out of bounds of image
            if (i < 0 || j < 0 || i >= width || j >= height)
            {
                sobelPos++;
                continue;
            }
            else
            {
                Gx = Gx + (sobelX[sobelPos] * image[i][j].rgbtRed);
                Gy = Gy + (sobelY[sobelPos] * image[i][j].rgbtRed);
                sobelPos++;
            }
        }
    }

    // Calculate square root of Gx squared and Gy squared
    rgbtRed = round(sqrt((Gx * Gx) + (Gy * Gy)));

    // Cap value at 255 and return
    return min(rgbtRed, 255);
}

int sobelGreen(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    int rgbtGreen = 0;
    int Gx = 0;
    int Gy = 0;

    // Determain starting and ending cords for loop
    int startPosX = x - 1;
    int startPosY = y - 1;
    int endPosX = x + 1;
    int endPosY = y + 1;

    // Initialize array containing order of Sobel operator numbers
    int sobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int sobelPos = 0;

    for (int i = startPosX; i <= endPosX; i++)
    {
        for (int j = startPosY; j <= endPosY; j++)
        {
            // Check if pixel is out of bounds of image
            if (i < 0 || j < 0 || i >= width || j >= height)
            {
                sobelPos++;
                continue;
            }
            else
            {
                Gx = Gx + (sobelX[sobelPos] * image[i][j].rgbtGreen);
                Gy = Gy + (sobelY[sobelPos] * image[i][j].rgbtGreen);
                sobelPos++;
            }
        }
    }

    // Calculate square root of Gx squared and Gy squared
    rgbtGreen = round(sqrt((Gx * Gx) + (Gy * Gy)));

    // Cap value at 255 and return
    return min(rgbtGreen, 255);
}

int sobelBlue(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    int rgbtBlue = 0;
    int Gx = 0;
    int Gy = 0;

    // Determain starting and ending cords for loop
    int startPosX = x - 1;
    int startPosY = y - 1;
    int endPosX = x + 1;
    int endPosY = y + 1;

    // Initialize array containing order of Sobel operator numbers
    int sobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int sobelPos = 0;

    for (int i = startPosX; i <= endPosX; i++)
    {
        for (int j = startPosY; j <= endPosY; j++)
        {
            // Check if pixel is out of bounds of image
            if (i < 0 || j < 0 || i >= width || j >= height)
            {
                sobelPos++;
                continue;
            }
            else
            {
                Gx = Gx + (sobelX[sobelPos] * image[i][j].rgbtBlue);
                Gy = Gy + (sobelY[sobelPos] * image[i][j].rgbtBlue);
                sobelPos++;
            }
        }
    }

    // Calculate square root of Gx squared and Gy squared
    rgbtBlue = round(sqrt((Gx * Gx) + (Gy * Gy)));

    // Cap value at 255 and return
    return min(rgbtBlue, 255);
}

// Swap pixels
void swap(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    RGBTRIPLE temp = image[x][y];
    image[x][y] = image[x][width - (y + 1)];
    image[x][width - (y + 1)] = temp;
}

int min(int x, int y)
{
    if (y < x)
    {
        return y;
    }
    else
    {
        return x;
    }
}

int max(int x, int y)
{
    if (y > x)
    {
        return y;
    }
    else
    {
        return x;
    }
}