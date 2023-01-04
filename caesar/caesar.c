#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int check_args(int argc, string argv[]);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    if (check_args(argc, argv))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);

    string text = get_string("Plaintext: ");

    int length = strlen(text);
    char cipher[length];
    for (int i = 0; text[i] != '\0'; i++)
    {
        cipher[i] = rotate(text[i], key);
    }

    printf("ciphertext: %.*s\n", length, cipher);
    return 0;
}

int check_args(int argc, string argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            return 1;
        }
    }

    return 0;
}

char rotate(char c, int key)
{
    const int N = 26;
    key %= N;

    if (isalpha(c))
    {
        c += key;
        if (!isalpha(c))
        {
            c -= N;
        }
        return c;
    }

    return c;
}