#include <cs50.h>
#include <stdio.h>

void print_line(int lenght, char value)
{
    for(int i = 0; i < lenght; ++i)
    {
        printf("%c", value);
    }
}

void print_pyramid_row(int width, int row, char fill, char gap)
{
    const int gap_lenght = 2;

    print_line(width - row - 1, gap);
    print_line(row + 1, fill);
    print_line(gap_lenght, gap);
    print_line(row + 1, fill);
    print_line(width - row - 1, gap);
}

void print_pyramid(int height, char fill, char gap)
{
    const int width = height;
    for(int i = 0; i < height; ++i)
    {
        print_pyramid_row(width, i, fill, gap);
        printf("\n");
    }
}

int main(void)
{
    int height;
    do
    {
        height = get_int("Enter the desired height of the pyramids that should be between 1 and 8. \n");
    }
    while(height > 8 || height < 1);

    print_pyramid(height, get_char("Enter fill\n"), get_char("Enter gap\n"));
}