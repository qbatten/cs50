#include <stdio.h>
#include <stdlib.h>


#define BLOCKSIZE 512 // size of FAT blocks on card, in bytes


int main(int argc, char *argv[])
{
	// check command-line args
	if (argc != 2)
	{
		printf("Usage: ./recover image");
		return 1;
	}

	// init vars
	char *inputFile = argv[1];
	printf("input file = %s\n", inputFile);

	char *currBlock = malloc(BLOCKSIZE); //current FAT block we're looking at from the card

	// open input file
	FILE *inptr = fopen("card.raw", "r");
	if (inptr == NULL)
	{
		printf("Could not open infile\n");
		return 2;
	}

	// read in 512 bytes
	fseek(inptr, (2*512), SEEK_SET);
	fread(currBlock, 512, 1, inptr);

	printf("%#x || %#x || %#x\n", currBlock[0], currBlock[1], currBlock[2]);

	if (currBlock[0] == 0xffffffff && currBlock[1] == 0xffffffd8 && currBlock[2] == 0xffffffff)
	{
		printf("beginJPEG\n");
	}

   // for (int i = 0; i < BLOCKSIZE; i++)
   // {
   // 	if(currBlock[i] == 0)
   // 	{

   // 	}
   // 	else
   // 	{
			// printf("%i", currBlock[i]);
   // 	}
   // }
	free(currBlock);

}
