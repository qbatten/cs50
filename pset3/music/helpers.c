// Helper functions for music

#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    //initialize variables
    int num = 0; //numerator
    int den = 0; //denominator

    //check input formatting (should be "X/Y", where x and y are both digits)
    if (isdigit(fraction[0]) == 0 || isdigit(fraction[2]) == 0)
    {
        printf("Error: Incorrect input to duration()\n");
        return 1;
    }

    //copy from input string into out int variables
    num = atoi(&fraction[0]);
    den = atoi(&fraction[2]);

    //adjust numerator to turn it into eighths.
    num = num * (8 / den);

    return num;
}



// Calculates frequency (in Hz) of a note
int frequency(string note)
{

    // Variables init
    int modOct = 0;
    int modAcc = 0;
    int modLetter = 0;
    int semi = 0;
    char *tNote = malloc(4 * sizeof(char));
    strncpy(tNote, note, 4);

    // Num of semitones away due to octave diff
    modOct = atoi(&tNote[strlen(tNote) - 1]) - 4;

    // Num of semitones away due to accidentals
    if (strlen(tNote) == 3)
    {
        char tempOctave = tNote[2];
        if (tNote[1] == 'b')
        {
            tNote[1] = '#';
            if (tNote[0] == 'A')
            {
                tNote[0] = 'G';
            }
            else if (tNote[0] == 'F' || tNote[0] == 'C')
            {
                printf("Error in frequency(), line 33\n");
            }
            else
            {
                tNote[0] = tNote[0] - 1;
            }
            tNote[2] = tempOctave;

        }
        tNote[2] = '\0';
    }
    else
    {
        tNote[1] = '\0';
    }

    //Num of semitones away due to Note Letter
    string cNote[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    for (int i = 0; i < 12; i++)
    {
        if (strcmp(tNote, cNote[i]) == 0)
        {
            modLetter = i - 9;
        }
    }


    //Calculate Num of Semitones total
    semi = modOct * 12 + modAcc + modLetter;

    //convert to Hz
    double tempD = 0;
    tempD = (double) semi;
    tempD = tempD / 12;
    tempD = pow(2, tempD);
    tempD = tempD * 440;
    return round(tempD);

}




// Determines whether a string represents a rest
bool is_rest(string s)
{
    int result;

    if (strcmp(s, "") == 0)
    {
        result = 1;
    }
    else
    {
        result = 0;
    }

    return result;
}
