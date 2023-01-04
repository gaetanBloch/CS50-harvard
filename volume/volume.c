// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <math.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

void copy_header(FILE *input, FILE *output);
void transform_wav(FILE *input, FILE *output, float factor);

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

    copy_header(input, output);
    transform_wav(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}

// Copy header from input file to output file
void copy_header(FILE *input, FILE *output)
{
    // do
    // {
    //     uint8_t *input_buffer = malloc(HEADER_SIZE * sizeof(uint8_t));
    // }
    // while (input_buffer != NULL);
    // free(input_buffer);

    uint8_t input_buffer[HEADER_SIZE];
    // Read the Header from the input file
    fread(input_buffer, sizeof(uint8_t), HEADER_SIZE, input);
    // Write the Header the output file
    fwrite(input_buffer, sizeof(uint8_t), HEADER_SIZE, output);
}

// Read samples from input file and write updated data to output file
void transform_wav(FILE *input, FILE *output, float factor)
{
    int16_t buffer = 0;
    int chunk_read = 0;

    while (true)
    {
        chunk_read = fread(&buffer, sizeof(int16_t), 1, input);

        if (chunk_read < 1)
        {
            if (ferror(input))
            {
                printf("Error reading from input file.\n");
                return;
            }
            else
            {
                // End of file reached
                break;
            }
        }

        buffer *= factor;
        fwrite(&buffer, sizeof(uint16_t), 1, output);
    }
}