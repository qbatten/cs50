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

    //load the number into an array of digits
    int digits[16];
    long long numConv = num;
    for (int i = numLength; i > 0; i--)
    {
        digits[i] = numConv % 10;
        numConv = (numConv - numConv % 10) / 10;
    }


    //CHECK 2: initial digits & assign cardtype
    if (numLength == 15 && digits[1] == 3 && (digits[2] == 4 || digits[2] == 7))
    {
        cardType = 1;
    }
    else if (numLength == 16 && digits[1] == 5 && digits[2] > 0 && digits[2] < 6)
    {
        cardType = 2;
    }
    else if ((numLength == 13 || numLength == 16) && digits[1] == 4)
    {
        cardType = 3;
    }
    else
    {
        printf("INVALID\n");
        exit(0);
    }

    //CHECK 3: Luhn's Algorithm
    int start = numLength - 1;
    int tempDigit = 0;

    for (int i = start; i > 0; i = i - 2)
    {

        digits[i] = digits[i] * 2;
        if (digits[i] > 9)
        {
            tempDigit = tempDigit + (digits[i] % 10) + 1;
        }
        else
        {
            tempDigit = tempDigit + digits[i];
        }
    }

    for (int i = numLength; i > 0; i = i - 2)
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
