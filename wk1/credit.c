#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long long num = get_long_long("Number: "); //get number from user
    int numLength = 0;
    int cardType = 0; //1 = Amex, 2 = Master, 3 = Visa
    int valid = 0; // is it valid?
    //we increment valid up by 1 when it passes a check.
    //there are 3 validity checks (length, first digits, Luhn's algorithm)
    //so if valid = 3, the number is good.


    //CHECK 1: length
    if (num > 999999999999 && num < 10000000000001) // 13 digit
    {
        numLength = 13;
        valid = valid + 1;
    }
    else if (num > 99999999999999 && num < 1000000000000001) // 15 digit
    {
        numLength = 15;
        valid = valid + 1;
    }
    else if (num > 999999999999999 && num < 10000000000000001) // 16 digit
    {
        numLength = 16;
        valid = valid + 1;
    }
    else
    {
        printf("INVALID\n");
        exit(0);
    }

    // Load the number into an array of digits.
    // We want the digits to go forward, so this loop needs to go backwards.
    // 	 1. I know there's a better way to do this than using numLength to tell the program when to stop reading from the array, but you cant 
    //	initialize an array using a variable and we haven't dug into memory stuff yet.
    //	 2. I start it at 1 (which isn't what you're supposed to do, right?) because I do a bunch of loops that count down and the alternative 
    //	    would be to make numLength smaller than it really is, which seems weird too. Not sure what the right answer is here, will think a bit 
    //	    more and come back.
    int digits[16];
    long long numConv = num;
    for (int i = numLength; i > 0; i--)
    {
        digits[i] = numConv % 10;
        numConv = (numConv - numConv % 10) / 10;
    }


    //CHECK 2: initial digits & assign cardtype
    if (numLength == 15 && digits[1] == 3 && (digits[2] == 4 || digits[2] == 7)) //
    {
    	//Amex numbers are 15 digits long and start w 24 or 27
        cardType = 1; 
    }
    else if (numLength == 16 && digits[1] == 5 && digits[2] > 0 && digits[2] < 6)
    {
    	//Mastercard numbers are 16 digits and start w 51, 52, 53, 54, or 55
        cardType = 2;
    }
    else if ((numLength == 13 || numLength == 16) && digits[1] == 4)
    {
    	//Visa numbers are 13 or 16 digits and start w 4
        cardType = 3;
    }
    else
    {
    	//If it doesn't fit those criteria, it's not valid!
        printf("INVALID\n");
        exit(0);
    }

    //CHECK 3: Luhn's Algorithm
    int start = numLength - 1; //start at the second-to-last digit
    int tempDigit = 0; //the sum we're getting to...will be the final result of the algorithm

    for (int i = start; i > 0; i = i - 2) //run thru every other digit starting w second-last digit
    {
        digits[i] = digits[i] * 2; // multiple each digit by 2
        if (digits[i] > 9) //if the result is a 2-digit number, add the digits. (aka add 1)
        {
            tempDigit = tempDigit + (digits[i] % 10) + 1;
        }
        else //if its a one-digit number, add that digit to tempDigit
        {
            tempDigit = tempDigit + digits[i];
        }
    }

    for (int i = numLength; i > 0; i = i - 2) //now do the other half of the digits, just add em each onto tempDigit
    {
        tempDigit = tempDigit + digits[i];
    }

    if (tempDigit % 10 == 0)
    {
        valid = valid + 1;
    }
    else
    {
        printf("INVALID\n");
    }

    //print card type... if we're still here, it's valid.
    switch (cardType)
    {
        case 1 :
            printf("AMEX\n");
            break;
        case 2 :
            printf("MASTERCARD\n");
            break;
        case 3 :
            printf("VISA\n");
            break;
        default :
            printf("ERROR\n");
    }
}
