#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	// ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: rtest infile outfile\n");
        return 1;
    }

    // remember input
    char *input = argv[1];

    printf("%s, len: %i\n", input, strlen(input));
}