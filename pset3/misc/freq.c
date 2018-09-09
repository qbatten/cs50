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

    //Num of semitones away due to octave diff
    modOct = atoi(&note[strlen(note) -1]) - 4;
    modOct = modOct * 7;

    //Num of semitones away due to accidentals
    if (strlen(note) == 3)
    {
        if (strcmp(&note[1],"#") == 0)
        {
            modAcc = 1;
        }
        else if (strcmp(&note[1],"b") == 0)
        {
            modAcc = -1;
        }
        else
        {
            printf("Error in frequency(), line 57\n");
            return 1;
        }
    }

    //Num of semitones away due to Note Letter
    modLetter = (int) note[0];
    int notesC[12] = {'C','','D'


    //Calculate Num of Semitones total
    semi = modOct + modAcc + modLetter;

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
