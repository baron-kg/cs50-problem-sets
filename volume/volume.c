// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Function prototypes
void copy_header(FILE *input_ptr, FILE *output_ptr);
void change_volume(FILE *input_ptr, FILE *output_ptr, float factor);

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    //Copy header from input file to output file
    copy_header(input, output);

    // TODO: Read samples from input file and write updated data to output file
    change_volume(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

void copy_header(FILE *input_ptr, FILE *output_ptr)
{
    uint8_t header_content[HEADER_SIZE];
    fread(header_content, HEADER_SIZE, 1, input_ptr);

    fwrite(header_content, HEADER_SIZE, 1, output_ptr);
}

void change_volume(FILE *input_ptr, FILE *output_ptr, float factor)
{
    int16_t sample = 0;

    while (fread(&sample, sizeof(int16_t), 1, input_ptr))
    {
        sample *= factor;

        fwrite(&sample, sizeof(int16_t), 1, output_ptr);
    }
}