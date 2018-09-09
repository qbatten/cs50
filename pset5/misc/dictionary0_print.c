// Implements a dictionary's functionality

// Libraries
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Other files
#include "dictionary.h"

// Global variables
#define HASHTABLE_SIZE 65536
node *hashTable[HASHTABLE_SIZE] = {NULL};

// Hash function
// Source: /u/delipity on cs50 subreddit (https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/)
int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    printf("%s", word);
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *inptr = fopen(dictionary, "r");

    // initialize tempvars
    char *currWord = malloc((LENGTH + 1) * sizeof(char));
    unsigned int currHash;
    node *newNode;
    node *checkNode;
    int count = 0;

    //read in a word
    while (fscanf(inptr, "%s", currWord) != EOF)
    {
        count++;
        bool tmpDone = 0;

        // hash it
        currHash = hash_it(currWord);

        // make a new node, fill in word
        newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("MemFull\n");
            return 1;
        }
        strncpy(newNode->word, currWord, LENGTH);


        // check if list has NOT been started
        if (hashTable[currHash] == NULL)
        {
            hashTable[currHash] = newNode;
            newNode->next = NULL;
            tmpDone = 1;
            printf("%i, %i, I, %s\n", count, currHash, hashTable[currHash]->word);
        }
        else
        {
            printf("%i, %i, P, ", count, currHash);
            // set pointers
            newNode->next = hashTable[currHash];
            checkNode = hashTable[currHash];

            // iterate thru linked list and place new node
            while (tmpDone == 0)
            {
                if (strncmp(newNode->word, checkNode->word, LENGTH) < 0) //currWord < next word
                {
                    // insert into linked list
                    printf("| %s |, %s\n", newNode->word, checkNode->word);
                    newNode->next = checkNode->next;
                    checkNode->next = newNode;
                    tmpDone = 1;
                }

                if (strncmp(newNode->word, checkNode->word, LENGTH + 1) > 0) //currWord > next word
                {
                    if (checkNode->next == NULL)
                    {
                        printf("%s, | %s | end\n", checkNode->word, newNode->word);
                        checkNode->next = newNode;
                        newNode->next = NULL;
                        tmpDone = 1;
                    }
                    else
                    {
                        // move along
                        printf("%s->", checkNode->word);
                        newNode->next = checkNode;
                        checkNode = checkNode->next;
                    }
                }

            }
        }

    }

    return 1;


}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
