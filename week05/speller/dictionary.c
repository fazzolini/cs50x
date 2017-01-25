// ./speller dictionaries/small texts/alice.txt
/**----------------------------------------------------------------------------
 * Do all imports
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

/**----------------------------------------------------------------------------
 * Define structs
 */
// define a node for trie
typedef struct _node
{
    bool is_word;
    struct _node *children[ALEPH];
}
node;

/**----------------------------------------------------------------------------
 * Global vars to be accessed by all methods
 */
node *root;

/** ---------------------------------------------------------------------------
 * prototypes
 */
int get_child_index(int c); // return index in trie based on character
void test_get_child_index();
void add_word(node *trie_root, char *word); // add word to trie
void print_child_ptrs(node *trie_node); // print addresses of pointers

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
    // open file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open the dictionary.\n");
        return 1;
    }
    
    // initialize root node of trie
    root = malloc(sizeof(node));
    root -> is_word = false;
    
    char *word = malloc(sizeof(int) * LENGTH);
    
    char *w = "abba";
    fprintf(stdout, "\nTest word is: %s\n", w);
    
    // test_get_child_index();
    
    while(fscanf(inptr, "%s", word) != EOF)
    {
        fprintf(stdout, "word is %s\n", word);
        add_word(root, word);
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

/**
 * Prints addresses of trie_node's children
 */
void print_child_ptrs(node *trie_node)
{
    for (int i = 0; i < ALEPH; i++)
    {
        if (trie_node->children[i] == NULL)
        {
            fprintf(stdout, "%d: NULL\n", i);
        }
        else
        {
            fprintf(stdout, "%d: %p\n", i, trie_node->children[i]);
        }
    }
}

/**
 * Returns index of child depending on character c
 */
int get_child_index(int c)
{
    int result;
    if (isalpha(c))
    {
        return tolower(c) - 97;
    }
    else
    {
        return 26; // 26 is for '
    }
    
    return result;
}

// for testing
void test_get_child_index()
{
    fprintf(stdout, "\nTesting test_get_child_index()\n");
    fprintf(stdout, "Printing char-index correspondence\n");
    for (char c = 97; c < 123; c++)
    {
        fprintf(stdout, "%c: %d\n", c, get_child_index(c));
    }
    fprintf(stdout, "%c: %d\n\n", '\'', get_child_index('\''));   
}

/**
 * Returns index of child depending on character c
 */
void add_word(node *trie_root, char *word)
{
    node *cursor = root;
    fprintf(stdout, "Adding the word \'%s\'\n\n", word);
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int child_index = get_child_index(word[i]);
        fprintf(stdout, "Char is %c, index is %d\n", word[i], child_index);
        
        // if node exists
        if (cursor->children[child_index] != NULL)
        {
            fprintf(stdout, "node for %c exists, going deeper...\n\n", word[i]);
            cursor = cursor->children[child_index];
        }
        else
        {
            fprintf(stdout, "node for %c doesn't exist, creating new node...\n", word[i]);
            cursor->children[child_index] = malloc(sizeof(node));
            fprintf(stdout, "node for %c exists now, going deeper...\n\n", word[i]);
            cursor = cursor->children[child_index];
        }
    }
    for (int i = 0; i < 80; i++) printf("-");
    printf("\n\n");
}