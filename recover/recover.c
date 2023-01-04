#include <stdint.h>
#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <math.h>

typedef uint8_t BYTE;

const BYTE FIRST_BYTE = 0xff;
const BYTE SECOND_BYTE = 0xd8;
const BYTE THIRD_BYTE = 0xff;

const int BLOCK_SIZE = 512;
const int FILE_NAME_SIZE = 8 * sizeof(BYTE);

bool validate_first_4_bits(BYTE value);
bool validate_first_3_bytes(BYTE *raw_file_buffer);
bool identify_jpeg(BYTE *raw_file_buffer);

int main(int argc, char *argv[])
{

    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open raw file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open raw file.\n");
        return 1;
    }

    int image_count = 0;
    BYTE raw_file_buffer[BLOCK_SIZE];
    char *image_name_buffer = NULL;
    FILE *image_file = NULL;
    while (fread(raw_file_buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (identify_jpeg(raw_file_buffer))
        {
            if (image_file != NULL)
            {
                fclose(image_file);
                free(image_name_buffer);
            }

            // Create file name
            image_name_buffer = malloc(FILE_NAME_SIZE);
            sprintf(image_name_buffer, "%03u.jpg", image_count++);
            printf("%s\n", image_name_buffer);

            // Open new jpg file
            image_file = fopen(image_name_buffer, "w");
        }

        if (image_file != NULL)
        {
            fwrite(raw_file_buffer, 1, BLOCK_SIZE, image_file);
        }
    }

    fclose(image_file);
    fclose(raw_file);
    free(image_name_buffer);

    return 0;
}

bool identify_jpeg(BYTE *raw_file_buffer)
{
    return validate_first_3_bytes(raw_file_buffer) &&
           validate_first_4_bits(raw_file_buffer[3]);
}

bool validate_first_3_bytes(BYTE *raw_file_buffer)
{
    return raw_file_buffer[0] == FIRST_BYTE &&
           raw_file_buffer[1] == SECOND_BYTE &&
           raw_file_buffer[2] == THIRD_BYTE;
}

bool validate_first_4_bits(BYTE value)
{
    // Bitwise AND Operation to check that the byte starts with 1110
    const uint8_t mask = 0b11110000;
    return (value & mask) == value;
}