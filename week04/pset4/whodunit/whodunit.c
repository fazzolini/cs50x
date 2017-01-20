
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
    BITMAPINFOHEADER bf;
    fread(&bf, sizeof(BITMAPINFOHEADER), 1, inputptr);
    
    // read the BITMAPINFOHEADER
    BITMAPFILEHEADER bi;
    fread(&bi, sizeof(BITMAPFILEHEADER), 1, inputptr);
    
    // enforce 24-bit color depth
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(inputptr);
        fclose(outputptr);
        fprintf(stderr, "Unsupported file type.\n");
        return 4;
    }
    
    // implement core functionality here
    
    
    // if success
    return 0;
}