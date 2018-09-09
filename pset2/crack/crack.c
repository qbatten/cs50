/* This function takes a hashed password from crypt and cracks it by guessing keys and trying them. The password must be
five characters or less, and the salt must be two characters—- this ensures that the less-secure crypt algorithm (DES) is
used
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>

/*
TEST
Compares strings aHash (actual hash) and the output of crypt(key, salt)
Input: salt is the salt pulled from the inputted hash; hash is the inputted hash. key is the current key guess.
Output: returns 0 if they match (aka the guessKey is correct); otherwise returns a non-zero int.
*/
int test(char *salt, char *key, char *hash)
{
    int result = 1;
    char *realHash = malloc(20 * sizeof(char));
    if (realHash == NULL)
    {
        printf("\nMemFull\n");
    }
    strncpy(realHash, hash, 15);
    char *guessHash = malloc(20 * sizeof(char));
    strncpy(guessHash, crypt(key, salt), 14);
    result = strcmp(realHash, guessHash);
    free(realHash);
    free(guessHash);
    return result;
}



/*
INCREMENT
Takes the key and the place, and increments the key up. The place is used so that it's recursive
    when a digit ticks over the edge and it calls itself for the next digit to the left, etc, til
    it's a successful increment.
Input: pntr to the guessKey, to be incremented; must be an editable string. Place, which is 1 when the function is started.
Output: none. Alters guessKey in its place.
*/
void increment(char *key, int place)
{
    int i = strlen(key) - place;
    switch (key[i])
    {
        case 57:
            key[i] = 65;
            break;
        case 90:
            key[i] = 97;
            break;
        case 122:
            key[i] = 46;
            increment(key, place + 1);
            if (place > strlen(key) || place < 0)
            {
                printf("\nPlace Overrun in Increment!\n\n");
                exit(0);
            }
            break;
        default:
            if ((key[i] >= 46 && key[i] < 57) ||
                (key[i] >= 65 && key[i] < 90) ||
                (key[i] >= 97 && key[i] < 122))
            {
                key[i]++;
            }
            else
            {
                printf("\nInvalid Value in Increment Fn!\n\n");
                exit(0);
            }
    }
}



int main(int argc, char *argv[])
{


    //Input Check:
    if (argc != 2)
    {
        printf("Incorrect # of arguments: hash\n");
        return 1;
    }
    else if (strlen(argv[1]) != 13)
    {
        printf("Must be a crypt hash, which is always 13 characters long.\n");
    }


    //Global Variables
    char *salt = malloc(3 * sizeof(char)); //INPUTTED SALT. init and correctly fill the salt var
    strncpy(salt, argv[1], 2);
    salt[2] = (char) "\0";

    char *aHash = malloc(14 * sizeof(char)); //INPUTTED HASH. init and correctly fill the actual hash (aHash) var.
    strncpy(aHash, argv[1], 14);

    char *gKey = malloc(6 * sizeof(char));  // Guessed key. This is where we store the current hash to be guessed
    char *gKeyInit5 = "....."; //Init keys for testing keys of diff lengths
    char *gKeyInit4 = "....";
    char *gKeyInit3 = "...";
    char *gKeyInit2 = "..";
    char *gKeyInit1 = ".";
    int match = 1; //whether the current variable is a match. 1 for NOT match, 0 for YES match
    int place = 1; //initial value for place in gKey array that we're currently implementing.


    //Setup for main loop
    strcpy(gKey, gKeyInit1); //fill malloc'ed gVars with initial vals


    //MAIN LOOP
    while (match != 0)
    {
        match = test(salt, gKey, aHash); //check for match btwn guess and real hash

        if (match == 0) //if it matches, print and exit
        {
            char *gHash = crypt(gKey, salt);
            printf("%s\n", gKey);
            return 0;
        }
        else if (strcmp(gKey, "z") == 0) //advances gKey from 1 to 2 to 3...5 digits.
        {
            strcpy(gKey, gKeyInit2);
        }
        else if (strcmp(gKey, "zz") == 0)
        {
            strcpy(gKey, gKeyInit3);
        }
        else if (strcmp(gKey, "zzz") == 0)
        {
            strcpy(gKey, gKeyInit4);
        }
        else if (strcmp(gKey, "zzzz") == 0)
        {
            strcpy(gKey, gKeyInit5);
        }
        else //if it's not a special case, increment like normal
        {
            increment(gKey, place);
        }
    }
    free(gKey);
}