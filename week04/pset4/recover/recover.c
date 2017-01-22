#include <stdio.h>
#include <cs50.h>

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
    
    // if OK return 0
    return 0;
}