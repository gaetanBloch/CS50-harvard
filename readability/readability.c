#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int compute_index(int letters, int words, int sentences);

int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    printf("l: %i\n", letters);
    int words = count_words(text);
    printf("w: %i\n", words);
    int sentences = count_sentences(text);
    printf("s: %i\n", sentences);
    int index = compute_index(letters, words, sentences);

    if (index < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }

    if (index > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }

    printf("Grade %i\n", index);
}

int count_letters(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }

    return ++count;
}

int count_sentences(string text)
{
    int count = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char c = text[i];
        if (c == '.' || c == '?' || c == '!')
        {
            count++;
        }
    }

    return count;
}

/** Calculate Coleman-Liau index */
int compute_index(int letters, int words, int sentences)
{
    double L = (letters * 100) / (double) words;
    double S = (sentences * 100) / (double) words;
    double index = 0.0588 * L - 0.296 * S - 15.8;
    return round(index);
}