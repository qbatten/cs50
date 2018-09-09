#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

#define KEYMAX 100 //This is the maximum number of character the program accepts as a Key.

/*
MAIN
INPUT: Command-line argument is just the "key" to the cipher. Must be alphabetic characters, nothing else.
OUTPUT: Prompts for plaintext (any alphnumeric chars or punctuation or spaces). Prints the resulting ciphertext.
*/
int main(int argc, char *argv[])
{
    //CHECK INPUT: number of arguments
    if (argc != 2)
    {
        printf("Usage: vig keyword\nKeyword must only contain alphabetic characters, no spaces, punctuation, or numbers.\n");
        return 1;
    }

    //CHECK INPUT: all chars are letters
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Usage: vig keyword\nKeyword must only contain alphabetic characters, no spaces, punctuation, or numbers.\n");
            return 1;
        }
    }

    //Keyword setup
    char *keyChar = malloc((KEYMAX + 1) * sizeof(char)); // array to hold editable input, len DOES incl \0
    if (keyChar == NULL)
    {
        printf("Malloc failure, line 32\n");
        return 1;
    }
    strncpy(keyChar, argv[1], KEYMAX);
    int keyNum[50] = {0};
    int keyLen = strlen(keyChar);

    for (int i = 0; i < keyLen; i++)
    {
        keyNum[i] = (int)(toupper(keyChar[i]) - 65);
    }

    //prompt for plaintext
    string plain = get_string("plaintext: ");
    if (plain == NULL)
    {
        printf("Malloc failure, line 50\n");
        return 1;
    }

    //main loop: setup
    char *output = malloc(KEYMAX * sizeof(char));
    if (output == NULL)
    {
        printf("Malloc failure\n");
        return 1;
    }
    int kPlace = 0; //this is the spot where we are in the keyword

    //main loop: the loop itself
    for (int pPlace = 0; pPlace < strlen(plain); pPlace++) //loop for PLAINTEXT
    {
        int tempInt = 0;
        int threshold = 0;
        if (kPlace >= keyLen)
        {
            kPlace = 0;
        }
        if (isalpha(plain[pPlace]) != 0)
        {
            tempInt = ((int) plain[pPlace]) + keyNum[kPlace];
            if (isupper(plain[pPlace]) != 0)
            {
                threshold = 90;
            }
            else if (islower(plain[pPlace]) != 0)
            {
                threshold = 122;
            }
            else
            {
                printf("\nERROR IN MAIN LOOP, line 88\n");
            }
            if (tempInt > threshold)
            {
                tempInt = tempInt - 26;
            }
            output[pPlace] = (char) tempInt;
            kPlace++;
        }
        else
        {
            output[pPlace] = plain[pPlace];
        }
    }

    //output result
    printf("ciphertext: %s\n", output);

    //free & return 0
    free(output);
    free(keyChar);
    return 0;
}