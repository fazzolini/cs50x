/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

// define bool
typedef enum { false, true } bool;

bool isInteger(char *number); // to check if n is indeed a digit

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy scale infile outfile\n");
        return 1;
    }

    int n;
    // make sure n is a valid integer and in the right interval
    if (isInteger(argv[1]))
    {
        n = atoi(argv[1]);
        fprintf(stdout, "n is %d\n", n);
        if (n < 1 || n > 100)
        {
            fprintf(stderr, "Scale argument should be an integer from 1 to 100.\n");
            return 43;
        }
        
    } else
    {
        fprintf(stderr, "Scale argument is not a valid integer.\n");
        return 42;
    }

    char *infile = argv[2];
    char *outfile = argv[3];

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
    
    // determine input padding for scanlines
    int in_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    /*  TO DO: change outfile's
        biWidth: width of image in pixels (no padding)
        biHeight: height of image in pixels
        biSizeImage: total size of image (pixels + padding)
        bfSize: total size of file
        */
    bi.biWidth *= n * bi.biWidth;
    bi.biHeight *= n * bi.biHeight;
    bi.biSizeImage = bi.biSizeImage; // no change for now
    bf.bfSize = bf.bfSize; // no change for now

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over input padding, if any
        fseek(inptr, in_padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < in_padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}


bool isInteger(char *number)
{
    if (number[0] == '0' && strlen(number) == 1)
    {
        return true;
    }
    if (number[0] < 49 || number[0] > 57)
    {
        // fprintf(stderr, "The first argument is not a valid integer.\n");
        return false;
    }
    for (int i=1, n=strlen(number); i < n; i++)
    {
        if ((number[i] < 48 || number[i] > 57))
        {
            // fprintf(stderr, "The first argument is not a valid integer.\n");
            return false;
        }
    }
    return true;
}