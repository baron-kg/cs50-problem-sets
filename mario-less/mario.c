#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    do
    {
        height = get_int("Enter the desired height of the pyramid: \n"); //Prompt the user for the height of the pyramid
    }
    while (height < 1 || height > 8);

    for (int i = 1; i <= height; ++i)
    {
        for (int j = 1; j <= (height - i); ++j) //Print the spaces
        {
            printf(" ");
        }

        for (int k = 1; k <= i; ++k) //Print the hashes
        {
            printf("#");
        }

        printf("\n");
    }
}