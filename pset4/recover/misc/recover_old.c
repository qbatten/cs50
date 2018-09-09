#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BLOCKSIZE 512 //size of FAT blocks on card, in bytes


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
	int jpegCount = 0; //number of JPEGs processed
//	char *jpegStartCheck = malloc(4 * sizeof(char));
	char *currBlock = malloc(BLOCKSIZE); //current FAT block we're looking at from the card
	char *currOutFile = malloc(8 * sizeof(char)); //current filename
	int tempTest = 1; //to be used throughout for error-checking tests

	// open input file
	FILE * inptr = fopen(inputFile, "r");
	if (inptr == NULL)
	{
		printf("Could not open infile\n");
		return 2;
	}
	fseek(inptr, 0, SEEK_SET);

	// initiate output file
	FILE * outptr;

	//Main loop
	while (jpegCount <= 51)
	{
		// output filename setup
		tempTest = 1;
		if (jpegCount < 9)
		{
			tempTest = sprintf(currOutFile, "00%i.jpg",jpegCount);
		}
		else
		{
			tempTest = sprintf(currOutFile, "0%i.jpg",jpegCount);
		}
		if (tempTest < 1)
		{
			printf("Error in output filename!\n");
			return 1;
		}

		char * nullTest = malloc(4);
		fread(nullTest, 4, 1, inptr);
		while ((nullTest[0] == 0x00000000 && nullTest[1] == 0x00000000 && nullTest[2] == 0x00000000) || strncmp(nullTest,"cs50", 4) == 0)
		{
			printf("skipping block\n");
			fread(nullTest, 4, 1, inptr);
			fseek(inptr, BLOCKSIZE - 4, SEEK_CUR);
			if (jpegCount == 50)
			{
				free(currBlock);
				return 0;
			}
		}

		// read in 512 bytes
		fseek(inptr,  -4, SEEK_CUR);
		fread(currBlock, BLOCKSIZE, 1, inptr);


		// check for jpeg start-bytes
		if (currBlock[0] == 0xffffffff && currBlock[1] == 0xffffffd8 && currBlock[2] == 0xffffffff)
		{
			// open new output file (w name based on jpegcount)
			outptr = fopen(currOutFile, "w");
			fwrite(currBlock, 512, 1, outptr);
			jpegCount++;

			printf("init block\n");
		}
		else
		{
			// write currBytes
			fwrite(currBlock, BLOCKSIZE, 1, outptr);
			printf("write block: %i\n", currBlock[0]);
		}

	}
	free(currBlock);
	return 0;

}
