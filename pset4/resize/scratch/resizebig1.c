// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy f infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // remember factor aka f
    int f = (*argv[1] - '0');

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for scanlines
    int paddingIn = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Edit file headers

    bi.biWidth = bi.biWidth * f;

    bi.biHeight = bi.biHeight * f;

    int paddingOut = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //int totalPaddingOut = paddingOut * bi.biHeight;

    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE) + paddingOut) * abs(bi.biHeight));

    bf.bfSize = bf.bfOffBits + bi.biSizeImage;

    printf("bi.biWidth: %i\nbi.biHeight: %i\npadding: %i\nbi.biSizeImage: %i\nbf.bfSize: %i\n",
            bi.biWidth, bi.biHeight, paddingOut, bi.biSizeImage, bf.bfSize);




    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        //temp storage of line
        RGBTRIPLE *currLine = malloc((bi.biWidth + paddingOut) * sizeof(RGBTRIPLE));

        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // copy RGB triple to currLine, f times
            for (int k = 0; k < f; k++)
            {
                (*(currLine + ((j + k) * sizeof(RGBTRIPLE)))) = triple;
            }
//////////////////////////////////////////////////////////////////
//currently here; working on debugging bc the outputted imagre is unreadable. think I might be writin

        }

        // write full line of RGBTRIPLES to outfile
        fwrite(&currLine, sizeof(currLine), 1, outptr);

        // skip over padding, if any
        fseek(inptr, paddingIn, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < paddingOut; k++)
        {
            fputc(0x00, outptr);
        }

    }

    // close infile
    fclose(inptr);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
