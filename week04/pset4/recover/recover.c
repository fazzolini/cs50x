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
        fprintf(stderr, "Could not open raw file \"%s\".\n", infile);
        return 2;
    } else
    {
        fprintf(stderr, "Successfully opened raw file \"%s\".\n", infile);
    }
    
    // declaration for output
    FILE *outptr;
    
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
        // check if this block is .jpg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // starts as jpg:
            // check if currently writing to .jpg
            if (jpeg_open)
            {
                // if writing
                // close file and open the next one again
                fclose(outptr);
                jpeg_open = false;
                fprintf(stderr, "Successfully closed \"%s\".\n", output_filename);
                i++;
                
                // make a filename
                sprintf(output_filename, "%03d.jpg", i);
                
                // open the file to write
                outptr = fopen(output_filename, "w");
                jpeg_open = true;
                // check if allright
                if (outptr == NULL)
                {
                    fprintf(stderr, "Could not open \"%s\" for writing.\n", output_filename);
                } else {
                    fprintf(stderr, "Successfully opened \"%s\" for writing.\n", output_filename);
                }
                
                // write current chunk to the open file
            } else 
            {
                // if not writing
                // then open new one
                
                // make a filename
                sprintf(output_filename, "%03d.jpg", i);
                
                // open the file to write
                outptr = fopen(output_filename, "w");
                jpeg_open = true;
                // check if allright
                if (outptr == NULL)
                {
                    fprintf(stderr, "Could not open \"%s\" for writing.\n", output_filename);
                } else {
                    fprintf(stderr, "Successfully opened \"%s\" for writing.\n", output_filename);
                }
                
                // write current chunk to the open file
            }
        } else
        {
            // if chunk does not start as .jpg
            // then check if currently writing
            if (jpeg_open)
            {
                // if writing
                // write current block
            } else
            {
                // if not writing
                // do nothing
            }
        }
    }
    
    // close .jpg file to which we were writing
    fclose(outptr);
    jpeg_open = false;
    
    // close file from which we were reading
    fclose(inptr);
    fprintf(stderr, "Successfully closed raw file \"%s\".\n", infile);
    
    // finish: finita la commedia
    
    // if OK return 0
    return 0;
}