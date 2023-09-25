#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *inputWAV = fopen(argv[1], "r");
    if (inputWAV == NULL)
    {
        printf("Cannot open %s\n", argv[1]);
        return 1;
    }

    // Read header
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, inputWAV);
    int headerPos = ftell(inputWAV);

    // Use check_format to ensure WAV format
    if (!check_format(header))
    {
        printf("File is not WAV!");
        return 1;
    }

    // Open output file for writing
    FILE *outputWAV = fopen(argv[2], "w");
    if (outputWAV == NULL)
    {
        printf("Failed to open %s\n", argv[2]);
        return 1;
    }

    // Write header to file
    fwrite(&header, sizeof(WAVHEADER), 1, outputWAV);

    // Use get_block_size to calculate size of block
    int blockSize = get_block_size(header);

    // Write reversed audio to file
    BYTE buffer[blockSize];

    fseek(inputWAV, blockSize, SEEK_END);

    while (ftell(inputWAV) - blockSize > headerPos)
    {
        fseek(inputWAV, (-2 * blockSize), SEEK_CUR);
        fread(&buffer, blockSize, 1, inputWAV);
        fwrite(&buffer, blockSize, 1, outputWAV);
    }

    fclose(inputWAV);
    fclose(outputWAV);
}

int check_format(WAVHEADER header)
{
    BYTE properHeader[4] = {57, 41, 56, 45};
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != properHeader[i])
        {
            return 1;
        }
    }
    return 0;
}

int get_block_size(WAVHEADER header)
{
    int blockSize = header.numChannels * (header.bitsPerSample / 8);
    return blockSize;
}