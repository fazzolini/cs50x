/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

// define bool
typedef enum { false, true } bool;

bool isInteger(char *number); // to check if string is an int
int getPadding(BITMAPINFOHEADER *bi); // calculate padding

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
        // fprintf(stdout, "n is %d\n", n);
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
    int in_padding = getPadding(&bi);
    
    // /*-------------------*/
    // fprintf(stdout, "old width: %d\n", bi.biWidth);
    // fprintf(stdout, "old padding: %d\n", in_padding);
    // fprintf(stdout, "old height: %d\n", bi.biHeight);
    // fprintf(stdout, "old size of image: %d\n", bi.biSizeImage);
    // fprintf(stdout, "old size of file: %d\n", bf.bfSize);
    // /*-------------------*/
    
    // keep track of original height and width
    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    
    /*  TO DO: change outfile's
        +biWidth: width of image in pixels (no padding)
        +biHeight: height of image in pixels
        +biSizeImage: total size of image (pixels + padding)
        +bfSize: total size of file
        */
    bi.biWidth = n * bi.biWidth;
    bi.biHeight = n * bi.biHeight;
    
    // new paddint for output scanline
    int out_padding = getPadding(&bi); // uses new, updated bi.Width
    // fprintf(stdout, "Padding after resize is %d\n", out_padding);
    
    bi.biSizeImage =  ((sizeof(RGBTRIPLE) * bi.biWidth) + out_padding) * abs(bi.biHeight);
    bf.bfSize =  bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // /*-------------------*/
    // fprintf(stdout, "new width: %d\n", bi.biWidth);
    // fprintf(stdout, "new padding: %d\n", out_padding);
    // fprintf(stdout, "new height: %d\n", bi.biHeight);
    // fprintf(stdout, "new size of image: %d\n", bi.biSizeImage);
    // fprintf(stdout, "new size of file: %d\n", bf.bfSize);
    // /*-------------------*/

    // write outfile's BITMAPFILEHEADER (which was changed above)
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER (which was changed above)
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    /*  TO DO: implement scaling functionality
        +resize pixels in a row (horizontal resizing)
        +resize rows (vertical resizing)
        implementing RECOPY METHOD
        */
    
    // for each row
    for (int i = 0; i < abs(oldHeight); i++)
    {
        // (1) for n-1 times
        for (int j = 0; j < n-1; j++)
        {
            // write pixels, padding to outfile
            
            // remember starting cursor position
            long starting_cursor = ftell(inptr);
            
            // iterate through all pixels in scanline
            for (int k = 0; k < oldWidth; k++)
            {
                // to store current pixel
                RGBTRIPLE triple;
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // writing read pixel n times to outfile (row resizing)
                for (int l = 0; l < n; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }                
            }

            // then add output padding
            for (int z = 0; z < out_padding; z++)
            {
                fputc(0x00, outptr);
            }
            
            // send infile cursor back to start of scanline
            fseek(inptr, starting_cursor, SEEK_SET);
        }

        // (2) write pixels, padding to outfile
        // iterate through all pixels in scanline
        for (int m = 0; m < oldWidth; m++)
        {
            // to store current pixel
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            // writing read pixel n times to outfile (row resizing)
            for (int t = 0; t < n; t++)
            {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }                
        }

        // then add output padding
        for (int q = 0; q < out_padding; q++)
        {
            fputc(0x00, outptr);
        }
        
        // (3) skip over infile padding
        fseek(inptr, in_padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

/*  this function checks if a string is an int
    */
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


/*  this function returns padding
    parameter passed by reference
    */
int getPadding(BITMAPINFOHEADER *bi)
{
    int padding;
    padding = (4 - ((*bi).biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    return padding;
}