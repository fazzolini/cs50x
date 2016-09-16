/**
 * mario.c
 * this is part of CS50X pset1
 *
 * Otto Brut
 * fazzolini@gmail.com
 *
 * version: 1.01
 *
 * Builds a mario-style pyramid.
 */

#include <cs50.h>
#include <stdio.h>

// prototype
void printLine(int line_width, int n_blocks_at_end);

int main(void)
{

    // dealing with input first
    int height;
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while ((height < 0) || (height > 23));
    
    // check for 0 border case
    if (height == 0)
    {
        // do nothing
        return 0;
    }
    
    // building the pyramid
    int width = height + 1;
    
    for (int i = 1; i <= height; i++)
    {
        printLine(width, i + 1);
    }
    
}

/*
 * this function prints a line of spaces
 * of a given line_width
 * with n_blocks_at_end 
 * pound signs at the end
 */
void printLine(int line_width, int n_blocks_at_end)
{

    int n_spaces = line_width - n_blocks_at_end;
    for (int i = 1; i <= line_width; i++)
    {
        if (i <= n_spaces)
        {
            printf(" ");
        } else
        {
            printf("#");
        }
    }
    printf("\n");

}