#include <stdio.h>
#include <stdint.h>
#include <string.h>

// to use with reading to buffer
typedef uint8_t  BYTE;
typedef enum { false, true } bool;

//
#define BUFFER_SIZE 512



int main(int argc, char *argv[])
{
    // check for valid inputs
    if (argc != 2)
    {
        fprintf(stderr, "Usage (only one argument): ./recover infile\n");
        return 1;
    }
    
    // remember filename to raw image
    char *infile = argv[1];
    
    // open the file
    FILE *inptr = fopen(infile, "r");
    // check for possible error
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open the file.\n");
        return 2;
    }
    
    // make a buffer to store pieces of data stream
    BYTE buffer[BUFFER_SIZE];
    
    // flag variables
    bool jpeg_open = false;
    
    // store a string for a filename (filename len not exceed 10)
    char output_filename[10];
    
    // loop through all 
    int i = 0;
    while (fread(&buffer, BUFFER_SIZE, 1, inptr))
    {
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // make a filename for output
            sprintf(output_filename, "%03d.jpg", i);
            // print output filename
            fprintf(stdout, "%s\n", output_filename);
            
            jpeg_open = true;
            
            
            
            i++;
        }
    }
    
    // if OK return 0
    return 0;
}