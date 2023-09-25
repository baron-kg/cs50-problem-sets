#include "helpers.h"
#include "stdio.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    int row_num = sizeof(image[0]) / sizeof(image[0][0]);
    int colum_num = sizeof(image[0]) / sizeof(image[0][1]);

    for (int i = 0; i < row_num; i++)
    {
        for (int j = 0; j < colum_num; j++)
        {
            if (image[i][j].rgbtRed == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtBlue == 0x00)
            {
                image[i][j].rgbtRed = 0xff;
            }
        }
    }
}
