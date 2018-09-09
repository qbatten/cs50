// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#include "bmp.h"


int main(int argc, char *argv[])
{
    // Ensure proper usage, number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy f infile outfile\n");
        return 1;
    }

    // Remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // Remember factor aka f
    int f = (*argv[1] - '0');

    // Ensure proper usage, factor size
    if (f < 1 || f > 100)
    {
        fprintf(stderr, "Resizing factor 'f' must be a positive integer between 1 and 100, inclusive\n");
        return 1;
    }

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Determine padding for scanlines and save info from input file
    int paddingIn = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int biWidthIn = bi.biWidth;
    int biHeightIn = abs(bi.biHeight);


    // Edit file headers
    bi.biWidth = bi.biWidth * f;

    bi.biHeight = bi.biHeight * f;

    int paddingOut = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    printf("PAD_OUT %i\n", paddingOut);

    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE) + paddingOut) * abs(bi.biHeight));
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    int lineRepeat = 0;
    int lineLength = biWidthIn * sizeof(RGBTRIPLE) + paddingIn;

    // iterate over infile's scanlines
    for (int i = 0; i < biHeightIn; i++)
    {

        // iterate over pixels in scanline
        for (int j = 0; j < biWidthIn; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile, f times
            for (int k = 0; k < f; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

        }

        // skip over padding, if any
        fseek(inptr, paddingIn, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < paddingOut; k++)
        {
            fputc(0x00, outptr);
        }

        lineRepeat++;

        if (lineRepeat >= f)
        {
            lineRepeat = 0;
        }
        else
        {
            fseek(inptr, -lineLength, SEEK_CUR);
            i--;
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
