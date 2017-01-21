
#include <stdio.h>
#include <cs50.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // check the number of arguments
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit inputfile outputfile.\n");
        return 1;
    }
    
    // filenames for input and output
    char *inputfile = argv[1];
    char *outputfile = argv[2];
    
    // open the file for reading
    FILE *inputptr = fopen(inputfile, "r");
    if (inputptr== NULL)
    {
        fprintf(stderr, "The input file could not be opened.\n");
        return 2;
    }
    
    // open the file for writing
    FILE *outputptr = fopen(outputfile, "w");
    if (outputptr == NULL)
    {
        fprintf(stderr, "The input file could not be opened.\n");
        return 3;
    }
    
    // read the BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inputptr);
    
    // read the BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inputptr);
    
    // enforce 24-bit color depth
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inputptr);
        fclose(outputptr);
        fprintf(stderr, "Unsupported file type.\n");
        return 4;
    }
    
    // update both headers for outfile
    // first the BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outputptr);
    
    // then the BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outputptr);
    
    // determine padding if necessary
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // iterate over infile's scanlines and add padding if necessary
    // this loop iterates through rows
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // this loop iterates trhrough pixels in each row
        for (int j = 0, biWidth = bi.biWidth; j < biWidth; j++)
        {
            // temp storage to keep current pixel
            RGBTRIPLE triple;
            
            // reag RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inputptr);
            
            // TO DO: perform trnaformation on the triple
            
            // write updated RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outputptr);
        }
        
        // skip over padding for reading further
        fseek(inputptr, padding, SEEK_CUR);
        
        // write the padding to output file
        for (int k=0; k<padding; k++)
        {
            fputc(0x00, outputptr);
        }
    }
    
    //close infile
    fclose(inputptr);
        
    // close outfile
    fclose(outputptr);
    
    // if success return
    return 0;
}