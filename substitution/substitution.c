#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int N = 26;
const int UPPER_SHIFT = 65;

int check_args(int argc, string argv[]);
int **create_mapping(string key);
char substitute(char c, int **mapping);

int main(int argc, string argv[])
{
    int code = check_args(argc, argv);
    if (code)
    {
        if (code == 1)
        {
            printf("Usage: ./substitution key\n");
        }
        else if (code == 2)
        {
            printf("Key must contain %i characters.\n", N);
        }
        else if (code == 3)
        {
            printf("Key must not contain duplicated characters.\n");
        }

        return 1;
    }

    string key = argv[1];

    int **mapping = create_mapping(key);

    string text = get_string("Plaintext: ");
    int length = strlen(text);
    char cipher[length];
    for (int i = 0; text[i] != '\0'; i++)
    {
        cipher[i] = substitute(text[i], mapping);
    }
    printf("ciphertext: %.*s\n", length, cipher);

    // free memory
    for (int i = 0; i < N; i ++)
    {
        free(mapping[i]);
    }
    free(mapping);

    return 0;
}

int check_args(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    if (strlen(argv[1]) != N)
    {
        return 2;
    }

    int chars[N];
    for (int i = 0; i < N; i++)
    {
        char current_char = argv[1][i];
        if (!isalpha(current_char))
        {
            return 1;
        }

        // Check char duplication
        for (int j = 0; j < i; j++)
        {
            if  (chars[j] == toupper(current_char))
            {
                return 3;
            }
        }
        chars[i] = toupper(current_char);
    }

    return 0;
}

int **create_mapping(string key)
{
    int **mapping = malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        mapping[i] = malloc(N * sizeof(int));
        mapping[i][i] = ((int)toupper(key[i])) - (UPPER_SHIFT + i);
    }
    return mapping;
}

char substitute(char c, int **mapping)
{
    if (isalpha(c))
    {
        int i = toupper(c) - UPPER_SHIFT;
        int value = c + mapping[i][i];
        if (islower(c))
        {
            return tolower(value);
        }
        return value;
    }
    return c;
}