#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Swap pixels
void swap(int height, int width, RGBTRIPLE image[height][width], int x, int y);

// Calculate blured pixel value
RGBTRIPLE blured_pixel(int height, int width, RGBTRIPLE image[height][width], int x, int y);

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate new RGB values using the sepia formula
            int sepiaRed = round((image[i][j].rgbtRed * 0.393) + (image[i][j].rgbtGreen * 0.769) + (image[i][j].rgbtBlue * 0.189));
            int sepiaGreen = round((image[i][j].rgbtRed * 0.349) + (image[i][j].rgbtGreen * 0.686) + (image[i][j].rgbtBlue * 0.168));
            int sepiaBlue = round((image[i][j].rgbtRed * 0.272) + (image[i][j].rgbtGreen * 0.534) + (image[i][j].rgbtBlue * 0.131));

            // Set existing RGB value to the new ones
            image[i][j].rgbtRed = min(sepiaRed, 255);
            image[i][j].rgbtGreen = min(sepiaGreen, 255);
            image[i][j].rgbtBlue = min(sepiaBlue, 255);
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

// Swap pixels
void swap(int height, int width, RGBTRIPLE image[height][width], int x, int y)
{
    RGBTRIPLE temp = image[x][y];
    image[x][y] = image[x][width - (y + 1)];
    image[x][width - (y + 1)] = temp; 
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
