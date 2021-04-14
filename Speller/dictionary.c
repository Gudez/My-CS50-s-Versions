// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

unsigned int n_words = 0; //count number of words in the dictionary

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
// You may change the value of N in dictionary.c, so that your hash table can have more buckets
const unsigned int N = 100;

// Hash table --> global pointer array: it represents the hash table you will use to keep track of words in the dictionary
node *table[N];

// FOURTH
// Returns true if word is in dictionary, else false
// MUST BE CASE INSENSITIVE
// should only return true for words actually in dictionary
// there are several assumptions to take (see site)

// is the word in the dictionary?
bool check(const char *word)
{

    // Converts string to its lowercase representation: my hashtable is case sensitive
    char copy[strlen(word) + 1];
    strcpy(copy, word);
    char *p = copy;
    for ( ; *p; ++p)
    {
        *p = tolower(*p);
    }

    // Find the word's position so I can iterate at the n-th node of the hash table
    unsigned int word_pos = hash(copy);

    // Initialize cursor that will iterate through the dictionary
    node *cursor = table[word_pos];

    // While I don't arrive at the end of the n-th position, iterate through the words
    while (cursor != NULL)
    {
        // Because I've just convert the word in lowercase I don't need strcasecmp()!
        if (strcmp(cursor->word, copy) == 0)
        {
            return true;
        }

        // Iterate
        cursor = cursor->next;
    }

    return false;
}

// SECOND
// Hashes word to a number
// it is going to take a word and run a hash function on it, returning some number that corresponds to that word
unsigned int hash(const char *word)
{
    int hash = 401;
    int c;

    while (*word != '\0')
	{
	    hash = ((hash << 4) + (int)(*word)) % N;
	    word++;
	}

	return hash % N;
}

// FIRST
// Loads dictionary into memory, returning true if successful, else false
// I have to load the files in the "dictionaries" folder
// You should made a little dictionary of your own to debugging

//!! it is responsible for loading all of the words in the dictionary into some sort of data structure (e.g. hash table)
bool load(const char *dictionary)
{

    // Open dictionary
    FILE *dictfile = fopen(dictionary, "r");

    // Check for problems
    if (!dictfile)
    {
        return false;
    }

    // Initialize the hash table --> It must point to the "ground" first
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Initialize the string that will contain the i-th input word of the dictionary
    char dictword[LENGTH];

    // While you are not at the end of the dictionary...

    while (fscanf(dictfile, "%s", dictword) != EOF)
    {
        // Create a node that will be "insert" into the hash table
        node *n = malloc(sizeof(struct node));

        // First I create the node, then I have to decide where to allocate it into the hash table
        // Check, if something is wrong free the memory and stop the process
        if (n == NULL)
        {
            unload();
            return false;
        }

        strcpy(n->word, dictword);
        n->next = NULL;

        // Find the position into the hash table
        unsigned int position = hash(dictword);

        // If the "base" point to another node, the current node will point to the first node
        if (table[position] != NULL)
        {
            n->next = table[position];
        }

        // And then I can assign the current node to the "base" --> I didn't lose anything!!
        table[position] = n;
        n_words++;

    }

    // Close the dictionary file
    fclose(dictfile);
    return true;
}

// THIRD
// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// how many words are in my dictionary?
unsigned int size(void)
{
    return n_words;
}

// FIFTH
// Unloads dictionary from memory, returning true if successful, else false
// any of the memory that I've had to allocate in order to store data inside of my data structure, must be freed.
bool unload(void)
{
    // Iterate through the "base" of the hash table
    for (int i = 0; i < N; i++)
    {
        // As before I use a "cursor variable" to free everything (aka the hash table)
        // I start from the beginning
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}