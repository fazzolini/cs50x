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

// to make a linked list of pointers to nodes in trie
typedef struct _ref
{
    node *trie_node;
    struct _ref *next;
}
ref;

/**----------------------------------------------------------------------------
 * Global vars to be accessed by all methods
 */
node *root;
bool debug = false; // for debugging
bool shortcut = true; // if true will calculate size of dict while loading the dict
int size_of_dict = 0; // for calculating size of dict while loading dict

/** ---------------------------------------------------------------------------
 * prototypes
 */
int get_child_index(int c); // return index in trie based on character
void test_get_child_index();
void add_word(node *trie_root, char *word); // add word to trie
void print_child_ptrs(node *trie_node); // print addresses of pointers
void add_ref(ref *start, node* t_node); // adds ref to linked list
ref * pop_a_ref_node(ref *start); // frees 1st element of linked list
void pop_a_trie_node(node *t_node); // frees t_node

/**----------------------------------------------------------------------------
 * ----------------------------------------------------------------------------
 * Implement functionality
 */
 
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node *cursor = root;
    int n = strlen(word);
    // check all letters before last one
    for (int i = 0; i < n - 1; i++)
    {
        int child_index = get_child_index(word[i]);
        if (cursor->children[child_index] != NULL)
        {
            cursor = cursor->children[child_index];
        }
        else
        {
            return false;
        }
    }
    // check last letter
    int last_child_index = get_child_index(word[n - 1]);
    if (cursor->children[last_child_index])
    {
        if (cursor->children[last_child_index]->is_word == true)
        {
            return true;
        }
    }
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
    root = calloc(1,sizeof(node));
    root -> is_word = false;
    
    char *word = calloc(1,sizeof(int) * LENGTH);
    
    while(fscanf(inptr, "%s", word) != EOF)
    {
        add_word(root, word);
    }
    
    free(word);
    fclose(inptr);
    
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
    if (shortcut)
    {
        // printf("SHORTCUT: printing size of dict: %d\n\n", size_of_dict);
        return size_of_dict;
    }
    // more hardcore and slow way to calculate size of dict
    else
    {
        int i = 0;
        
        ref *start = malloc(sizeof(ref));
        start->trie_node = root;
        while (start != NULL)
        {
            if (start->trie_node->is_word == true) i++;
            for (int i = 0; i < ALEPH; i++)
            {
                if (start->trie_node->children[i] != NULL)
                {
                    // adding pointer to node to list of nodes
                    add_ref(start, start->trie_node->children[i]);
                }
            }
            start = pop_a_ref_node(start); // removes first element if list of refs
        }
        free(start);
        return i;
    }
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
        ref *start = calloc(1,sizeof(ref));
        start->trie_node = root;
        // make a linked list of refs to all nodes in trie
        while (start != NULL)
        {
            for (int i = 0; i < ALEPH; i++)
            {
                if (start->trie_node->children[i] != NULL)
                {
                    // adding pointer to node to list of nodes
                    add_ref(start, start->trie_node->children[i]);
                }
            }
            pop_a_trie_node(start->trie_node); // free the node in trie
            start = pop_a_ref_node(start); // free first element if list of refs
        }
        free(start);
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
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        int child_index = get_child_index(word[i]);
        
        // if node exists
        if (cursor->children[child_index] != NULL)
        {
            // check if last letter
            if (i == n - 1)
            {
                cursor = cursor->children[child_index];
                cursor->is_word = true;
                size_of_dict++;

            }
            else
            {
                cursor = cursor->children[child_index];                
            }
        }
        else
        {
            // check if last letter
            if (i == n - 1)
            {
                cursor->children[child_index] = calloc(1,sizeof(node));
                cursor = cursor->children[child_index];
                cursor->is_word = true;
                size_of_dict++;
            }
            else
            {
                cursor->children[child_index] = calloc(1,sizeof(node));
                cursor = cursor->children[child_index];
                cursor->is_word = false;
            }
        }
    }
}

/**
 * takes a pointer to linked list of refs
 * and a pointer to trie node
 * and adss the pointer to trie node
 * to the SECOND POSITION of linked list
 * of refs
 */
void add_ref(ref *start, node* t_node)
{
    ref *new_ref = calloc(1,sizeof(ref));
    new_ref->trie_node = t_node;
    new_ref->next = start->next;
    start->next = new_ref;
}


/**
 * returns a pointer to the first
 * pointer to trie node in list of
 * refs, removes the first node from
 * list of refs, frees memory
 */
ref * pop_a_ref_node(ref *start)
{
    ref *cursor = start; // pointer for freeing memory
    ref * result = start->next;
    free(cursor);
    return result;
}

void pop_a_trie_node(node *t_node) // frees t_node
{
    free(t_node);
}