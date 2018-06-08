#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //get height
    int height = get_int("Height: ");

    //error check: height is w/in (0,23)
    while (height > 23 || height < 0)
    {
        height = get_int("Height: ");
    }

    //loop that goes thru all the lines
    for (int currLine = 1; currLine <= height; currLine++)
    {
        //loop that prints the first half
        for (int i = height; i > 0; i--)
        {
            int whichChar = i - currLine;
            if (whichChar > 0)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        printf("  "); //print the middle space

        //loop that prints the second half
        for (int i = 0; i < height; i++)
        {
            int whichChar = i - currLine;
            if (whichChar < 0)
            {
                printf("#");
            }
        }
        //end line
        printf("\n");
    }
}
