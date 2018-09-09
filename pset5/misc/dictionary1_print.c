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
    for (int i = 0, n = strlen(needs_hashing); i < n; i++)
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
    if (inptr == NULL)
    {
        return 0;
    }

    // initialize tempvars
    char *currWord = malloc((LENGTH + 1) * sizeof(char)); // word we just read in and are trying to import to dict
    unsigned int currHash; // hash of currWord
    node *newNode; // node we are placing (containts currWord)
    node *checkNode;
    bool tmpDone = 0; // used to break loop once word is placed
    int count = 0;

    //read in all words, one at a time
    while (fscanf(inptr, "%s", currWord) != EOF)
    {

        if (strcmp(currWord, "abbado") == 0 || strcmp(currWord, "abbess") == 0)
        {
            printf("boop\n");
        }

        // hash it
        currHash = hash_it(currWord);
        printf("%i, %i, %s", count, currHash, currWord);
        // make a new node, fill in word, init next to NULL, fill it w currWord and a null pointer
        newNode = malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("ERROR!\n");
            return 0;
        }

        strncpy(newNode->word, currWord, strlen(currWord));
        newNode->next = NULL;


        // check if list has been started. If not, place word, If there is a list, find the right spot for new word
        if (hashTable[currHash] == NULL)
        {
            printf(", I\n");
            hashTable[currHash] = newNode;
        }
        else if (strncmp(newNode->word, hashTable[currHash]->word, strlen(newNode->word)) < 0)
        {
            // insert into FIRST place in linked list
            printf(", F, -> %s ...\n", newNode->next->word);
            newNode->next = hashTable[currHash];
            hashTable[currHash] = newNode;
        }
        else
        {
            // search setup
            newNode->next = hashTable[currHash];

            // iterate thru linked list and place new node
            tmpDone = 0;
            while (tmpDone == 0)
            {
                //currWord > next word
                if (strncmp(newNode->word, newNode->next->word, strlen(newNode->word)) > 0)
                {
                    //if that's the end, place it at the end
                    if (newNode->next->next == NULL)
                    {
                        printf(", E\n");
                        newNode->next->next = newNode;
                        newNode->next = NULL;
                        tmpDone = 1;
                    }
                    else if (strncmp(newNode->word, newNode->next->next->word, strlen(newNode->word)) < 0)
                    {
                        // insert into linked list
                        printf("| %s |, %s ...\n", newNode->word, newNode->next->word);
                        checkNode = newNode->next->next;
                        newNode->next->next = newNode;
                        newNode->next = checkNode;

                        tmpDone = 1;
                    }
                    else // move to the next word
                    {
                        printf(" -> %s", newNode->next->word);
                        newNode->next = newNode->next->next;
                    }
                }
                else //shouldn't happen... it'd mean we have a duplicate word!
                {
                    printf("ERROR\nERROR\n");
                }


            }
        }
        count++;
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
    node *eraser;
    node *hold;
    int currList;
    int done;
    printf("\n\n\nUNLOADING...\n");
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        currList = i;
        printf("\n%i (%i): ", currList, (int) hashTable[currList]);
        if (hashTable[currList] != NULL)
        {
            eraser = hashTable[currList];
            hold = eraser->next;
            printf("%s -> ", eraser->word);
            free(eraser);
            done = 0;
            while (done == 0)
            {
                if (hold == NULL)
                {
                    printf(" | END | ");
                    done = 1;
                }
                else
                {
                    eraser = hold;
                    hold = eraser->next;
                    printf("%s -> ", eraser->word);
                    free(eraser);
                }

             }
         }
    }
    return true;
}
