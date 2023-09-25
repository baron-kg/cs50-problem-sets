#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

// Function declarations
bool startOfJPG(BYTE buffer[]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open card.raw file
    char *fileName = argv[1];
    FILE *inputFilePointer = fopen(fileName, "r");

    // Check if file can be opened
    if (inputFilePointer == NULL)
    {
        printf("Cannot open %s\n", fileName);
        return 2;
    }

    // Define variables to be used
    int blockSize = 512;
    int numberOfJPGs = 0;
    char filename[8];
    FILE *imgPointer = NULL;
    BYTE buffer[512];

    while (fread(buffer, 1, blockSize, inputFilePointer) == blockSize)
    {
        if (startOfJPG(buffer) == true)
        {
            // If not first JPG close previously opened file
            if (!(numberOfJPGs == 0))
            {
                fclose(imgPointer);
            }

            // Create new JPG file
            sprintf(filename, "%03i.jpg", numberOfJPGs);
            numberOfJPGs++;
            // Open new JPG and write data
            imgPointer = fopen(filename, "w");
        }

        if (!(numberOfJPGs == 0))
        {
            fwrite(buffer, blockSize, 1, imgPointer);
        }
    }

    fclose(imgPointer);
    fclose(inputFilePointer);

    return 0;
}

// Check if the new stream of data contains new JPG header
bool startOfJPG(BYTE buffer[])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
    {
        return true;
    }
    return false;
}