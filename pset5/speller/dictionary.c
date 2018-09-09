// Implements a dictionary's functionality

// Libraries
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Other files
#include "dictionary.h"

// Global variables
int count = 0;
#define HASHTABLE_SIZE 65536
node *hashTable[HASHTABLE_SIZE] = {0};

// Hash function
// Source: /u/delipity on cs50 subreddit (https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/)
int hash_it(char *needs_hashing)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(needs_hashing); i < n; i++)
    {
        hash = (hash << 2) ^ needs_hashing[i];
    }
    return hash % HASHTABLE_SIZE;
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //initialize vars
    char *inWord = malloc((LENGTH + 1) * sizeof(char));
    strncpy(inWord, word, LENGTH + 1);
    int currHash = 0;
    node *currNode = NULL;

    // lowercase it all
    for (int i = 0; i < strlen(inWord); i++)
    {
        inWord[i] = tolower(inWord[i]);
    }

    // hash the word
    currHash = hash_it(inWord);

    // search setup
    if (hashTable[currHash] == NULL)
    {
        free(inWord);
        return false;
    }
    currNode = hashTable[currHash];

    // iterate thru linked list and place new node
    // tmpDone = 0;
    while (true)
    {

        if (strncmp(inWord, currNode->word, LENGTH) > 0)
        {
            //if that's the end, return false
            if (currNode->next == NULL)
            {
                free(inWord);
                return false;
            }
            else // move to the next word
            {
                currNode = currNode->next;
            }
        }
        else if (strncmp(inWord, currNode->word, LENGTH) < 0)
        {
            free(inWord);
            return false;
        }
        else if (strncmp(inWord, currNode->word, LENGTH) == 0)//shouldn't happen... it'd mean we have a duplicate word!
        {
            free(inWord);
            return true;
        }
    }
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        count = 0;
        return 0;
    }

    // initialize tempvars
    char *currWord = malloc((LENGTH + 1) * sizeof(char)); // word we just read in and are trying to import to dict
    if (!currWord)
    {
        count = 0;
        // printf("ERROR\n");
        return 0;
    }

    unsigned int currHash = 0; // hash of currWord
    node *newNode = NULL; // node we are placing (containts currWord)
    node *checkNode = NULL;
    bool tmpDone = 0; // used to break loop once word is placed

    //read in all words, one at a time
    while (fscanf(inptr, "%s", currWord) != EOF)
    {
        // hash it
        currHash = hash_it(currWord);

        // make a new node, fill in word, init next to NULL, fill it w currWord and a null pointer
        newNode = malloc(sizeof(node));
        if (!newNode)
        {
            count = 0;
            // printf("ERROR!\n");
            return 0;
        }
        for (int i = 0; i < (LENGTH + 1); i++)
        {
            newNode->word[i] = 0;
        }
        newNode->next = NULL;
        strncpy(newNode->word, currWord, strlen(currWord));



        // check if list has been started. If not, place word, If there is a list, find the right spot for new word
        if (hashTable[currHash] == NULL)
        {
            // printf(", I\n");
            hashTable[currHash] = newNode;
        }
        else if (strncmp(newNode->word, hashTable[currHash]->word, strlen(newNode->word)) < 0)
        {
            // insert into FIRST place in linked list
            // printf(", F, -> %s ...\n", newNode->next->word);
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
                        // printf(", E\n");
                        newNode->next->next = newNode;
                        newNode->next = NULL;
                        tmpDone = 1;
                    }
                    else if (strncmp(newNode->word, newNode->next->next->word, strlen(newNode->word)) < 0)
                    {
                        // insert into linked list
                        // printf("| %s |, %s ...\n", newNode->word, newNode->next->word);
                        checkNode = newNode->next->next;
                        newNode->next->next = newNode;
                        newNode->next = checkNode;

                        tmpDone = 1;
                    }
                    else // move to the next word
                    {
                        // printf(" -> %s", newNode->next->word);
                        newNode->next = newNode->next->next;
                    }
                }
                else //shouldn't happen... it'd mean we have a duplicate word!
                {
                    // printf("ERROR\n");
                    count = 0;
                }


            }
        }
        count++;
    }
    fclose(inptr);
    free(currWord);
    return 1;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *eraser;
    node *hold;
    int currList;
    int done;
    // printf("\n\n\nUNLOADING...\n");
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        currList = i;
        // printf("\n%i (%i): ", currList, (int) hashTable[currList]);
        if (hashTable[currList] != NULL)
        {
            eraser = hashTable[currList];
            hold = eraser->next;
            // printf("%s -> ", eraser->word);
            free(eraser);
            done = 0;
            while (done == 0)
            {
                if (hold == NULL)
                {
                    // printf(" | END | ");
                    done = 1;
                }
                else
                {
                    eraser = hold;
                    hold = eraser->next;
                    // printf("%s -> ", eraser->word);
                    free(eraser);
                }

            }
        }
    }
    return true;
}
