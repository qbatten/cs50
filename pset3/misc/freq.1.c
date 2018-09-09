#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    string note = get_string("note: ");
    //Variables init
    int modOct = 0;
    int modAcc = 0;
    int modLetter = 0;
    int semi = 0;
    char *tNote = malloc(4 * sizeof(char));
    strncpy(tNote, note, 4);

    //Num of semitones away due to octave diff
    modOct = atoi(&tNote[strlen(tNote)-1]) - 4;

    //Num of semitones away due to accidentals
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
                tNote[0] = tNote[0] + 1;
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
    string cNote[12] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

    for (int i = 0; i < 12; i++)
    {
        if (strcmp(tNote, cNote[i]) == 0)
        {
            modLetter = i - 9;
        }
    }


    //Calculate Num of Semitones total
    semi = modOct * 12 + modAcc + modLetter;

    printf("oct %i; acc %i; let %i; total %i\n", modOct, modAcc, modLetter, semi);

    //convert to Hz
    double tempD = 0;
    tempD = (double) semi;
    tempD = tempD / 12;
    tempD = pow(2, tempD);
    tempD = tempD * 440;
    printf("\nAns: %f\n", round(tempD));
    return 0;

}
