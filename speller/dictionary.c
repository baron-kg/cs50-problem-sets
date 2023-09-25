// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 500;

// Hash table
node *table[N];

// Number of loaded words
int wordsNum = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Obtain hachCode
    unsigned int hashCode = hash(word);

    // Create new pointer used to move along the linked list
    node *pos = table[hashCode];

    // While pos is not pointing to NULL compare strings
    while (pos != NULL)
    {
        if (strcasecmp(word, pos->word) == 0)
        {
            return true;
        }
        pos = pos->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;

    int wordLen = strlen(word);
    for (int i = 0; i < wordLen; i++)
    {
        sum = sum + (toupper(word[i]) - 'A');
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *open = fopen(dictionary, "r");
    if (open == NULL)
    {
        printf("Unable to open file.\n");
        return false;
    }

    char buffer[LENGTH];
    while (fscanf(open, "%s", buffer) != EOF)
    {
        // Allocate memoty for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Unable to allocate memory for node!\n");
            return false;
        }

        // Copy string that was read from file to new node and set it's pointer to NULL
        strcpy(n->word, buffer);
        n->next = NULL;

        // Keep track of number of words loaded
        wordsNum++;

        // Initialize new var with a hash code form the hash function
        unsigned int hashCode = hash(n->word);

        // If the given position is empty place new node there
        if (table[hashCode] == NULL)
        {
            table[hashCode] = n;
        }
        // Else point new node to current node and then place the new node in its place
        else
        {
            n->next = table[hashCode];
            table[hashCode] = n;
        }
    }
    // Close the opened file and return true
    fclose(open);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return golobal var updated in load function
    return wordsNum;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
