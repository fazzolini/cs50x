// ./speller dictionaries/small texts/alice.txt
/**----------------------------------------------------------------------------
 * Do all imports
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

/**----------------------------------------------------------------------------
 * Define structs
 */
// define a node for trie
typedef struct _node
{
    bool is_word;
    struct _node *children[27];
}
node;

/**----------------------------------------------------------------------------
 * Global vars to be accessed by all methods
 */
node *root;

/** ---------------------------------------------------------------------------
 * prototypes
 */
int get_trie_index(int c); // return index in trie based on character
void add_word(node *trie_root, char *word); // add word to trie

/**----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * Implement functionality
 */
 
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    printf("root address is: %p\n", root);
    printf("root value is %d\n", root->is_word);
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    // open dict for reading
    // while there are words
    // print word
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open the dictionary.\n");
        return 1;
    }
    
    char *word = malloc(sizeof(int) * LENGTH);
    
    while(fscanf(inptr, "%s", word) != EOF)
    {
        fprintf(stdout, "word is %s\n", word);
    }
    
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}