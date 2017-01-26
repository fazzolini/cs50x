// ./speller dictionaries/small texts/austinpowers.txt
// ./speller texts/austinpowers.txt
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
bool debug = false;

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
    if (debug) printf("Checking if word \'%s\' is in the trie\n\n", word);
    node *cursor = root;
    int n = strlen(word);
    // check all letters before last one
    for (int i = 0; i < n - 1; i++)
    {
        int child_index = get_child_index(word[i]);
        if (cursor->children[child_index] != NULL)
        {
            if (debug) printf("%d: %c is in trie, going deeper\n", i+1, word[i]);
            cursor = cursor->children[child_index];
        }
        else
        {
            if (debug) printf("Word is NOT IN DICTIONARY\n\n");
            return false;
        }
    }
    // check last letter
    int last_child_index = get_child_index(word[n - 1]);
    if (cursor->children[last_child_index])
    {
        if (debug) printf("Last letter \'%c\' is in trie, checking if endword\n", word[n - 1]);
        if (cursor->children[last_child_index]->is_word == true)
        {
            if (debug) printf("Word is IN DICTIONARY ^__^\n\n");
            return true;
        }
    }
    if (debug) printf("Word is NOT IN DICTIONARY\n\n");
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
    if (debug) fprintf(stdout, "\nTest word is: %s\n", w);
    
    // test_get_child_index();
    
    while(fscanf(inptr, "%s", word) != EOF)
    {
        if (debug) fprintf(stdout, "word is %s\n", word);
        add_word(root, word);
    }
    
    // char *cat = "cat";
    // char *car = "car";
    // char *pil = "caterpillar";
    // printf("Checking %s:\n", cat);
    // check(cat);
    // printf("Checking %s:\n", car);
    // check(car);
    // printf("Checking %s:\n", pil);
    // check(pil);
    // printf("Checking %s:\n", "hobby");
    // check("hobby");
    
    if (debug)
    {
        return false;
    }
    else
    {
        return true;
    }
    
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
    if (debug)
    {
        return false;
    }
    else
    {
        return true;
    }
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
            if (debug) fprintf(stdout, "%d: NULL\n", i);
        }
        else
        {
            if (debug) fprintf(stdout, "%d: %p\n", i, trie_node->children[i]);
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
    if (debug) fprintf(stdout, "Adding the word \'%s\'\n\n", word);
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int child_index = get_child_index(word[i]);
        if (debug) fprintf(stdout, "Char is %c, index is %d\n", word[i], child_index);
        
        // if node exists
        if (cursor->children[child_index] != NULL)
        {
            // check if last letter
            if (i == n - 1)
            {
                if (debug) fprintf(stdout, "node for %c exists, going deeper...\n", word[i]);
                cursor = cursor->children[child_index];
                if (debug) fprintf(stdout, "%c is the LAST letter of the word, changing to \'true\'\n\n", word[i]);
                cursor->is_word = true;

            }
            else
            {
                if (debug) fprintf(stdout, "node for %c exists, going deeper...\n\n", word[i]);
                cursor = cursor->children[child_index];                
            }
        }
        else
        {
            // check if last letter
            if (i == n - 1)
            {
                if (debug) fprintf(stdout, "no node for %c, creating new node...\n", word[i]);
                cursor->children[child_index] = malloc(sizeof(node));
                if (debug) fprintf(stdout, "node for %c exists now, going deeper...\n", word[i]);
                cursor = cursor->children[child_index];
                if (debug) fprintf(stdout, "%c is the LAST letter of the word, changing to \'true\'\n\n", word[i]);
                cursor->is_word = true;
            }
            else
            {
                if (debug) fprintf(stdout, "no node for %c, creating new node...\n", word[i]);
                cursor->children[child_index] = malloc(sizeof(node));
                if (debug) fprintf(stdout, "node for %c exists now, going deeper...\n", word[i]);
                cursor = cursor->children[child_index];
                if (debug) fprintf(stdout, "writing default is_word for new node to \'false\'\n\n");
                cursor->is_word = false;
            }
        }
    }
    if (debug)
    {
        for (int i = 0; i < 80; i++) printf("-");
    }
    if (debug) printf("Added word \'%s\' to the trie\n\n", word);
}