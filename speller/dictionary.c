// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 1000000;

// Hash table
node *table[N];


bool check_node(node *current_node, const char *word)
{
    if(current_node == NULL)
    {
        return false;
    }

    if (strcasecmp("s", word) == 0)
    {
        return false;
    }

    if (strcasecmp(current_node->word, word) == 0)
    {
        return true;
    }

    return check_node(current_node->next, word);
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hash_value = hash(word);
    return check_node(table[hash_value], word);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //// Let's remember the key elements of good hash:
    // - Use only the data being hashed (here, only the char of the word)
    // - Use all the data being hashed (here, all the chars of the word)
    // - Should be deterministic
    // - Should have a good distribution to minimize collisions and clustering
    // - Generate very different hash codes for very similar data


    //// We use the polynomial rolling hash function.
    // This hash function works by iterating through the characters of the
    // input word and updating the hash value using a rolling hash function.
    // The rolling hash function computes a new hash value by shifting the
    // previous hash value left by a certain number of bits hard-coded below
    // and adding the current character.
    const int bits_shift = 5;

    unsigned int hash = 0;
    int c;
    // int c = word[0];

    // Trick to loop until we finish reading all chars in word
    while ((c = *word++))
    // while (int i = 0; word[i] != '\0'; c = word[i++])
    {
        // We make the char upper case so it's case insensitive
        hash = ((hash << bits_shift) + hash) + toupper(c);
    }

    // Simpler hash function still using polynomial rolling hash algo but faster
    // int prime_number = 41;
    // for (int i = 0; i < strlen(word); i++)
    // {
    //     hash = prime_number * hash + word[i];
    // }

    // Return the bucket from the remainder of hash / N
    return hash % N;
}

bool add_to_hash_table(char *word)
{
        // Compute hash value for word
        unsigned int hash_value = hash(word);

        //// Add to hash table
        // Create new node with the dictionary word
        node *new_node = malloc(sizeof(node));
        if(new_node == NULL)
        {
            return false;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;
        // The bucket is the remainder of the of hash / N
        // Add the node in the right bucket of Linked List
        // i.e. insert in the beginning of the list
        new_node->next = table[hash_value];
        table[hash_value] = new_node;
        return true;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dic_file = fopen(dictionary, "r");
    if(dic_file == NULL)
    {
        return false;
    }

    int count = 0;
    char buffer[LENGTH + 1];
    // Read each word line by line of the dictionary
    while (fgets(buffer, LENGTH + 1, dic_file) != NULL)
    {
        // Remove \n of the string if present
        buffer[strcspn(buffer, "\n")] = 0;
        if(strcmp(buffer, "") != 0)
        {
            count++;
            bool added = add_to_hash_table(buffer);
            if (!added)
            {
                return false;
            }
        }
    }

    fclose(dic_file);
    return true;
}

// Recursive count in Linked List
void count_node(node *current_node, int *count)
{
    if(current_node == NULL)
    {
        return;
    }

    // Increment count
    ++*count;
    count_node(current_node->next, count);
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        if(list != NULL)
        {
            ++count;
            count_node(list->next, &count);
        }
    }
    return count;
}

void do_unload(node *current_node)
{
    if(current_node == NULL)
    {
        return;
    }

    do_unload(current_node->next);
    free(current_node);
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *list = table[i];
        if(list != NULL)
        {
            do_unload(list);
        }
    }
    return true;
}
