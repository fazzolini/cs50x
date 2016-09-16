/**
 * water.c
 * this is part of CS50X pset1
 *
 * Otto Brut
 * fazzolini@gmail.com
 *
 * version: 1.01
 *
 * Converts minutes that you take shower
 * to bottles of water you use.
 */

#include <cs50.h>
#include <stdio.h>

int main(void)
{

	// handle user input
    printf("minutes: ");
    int minutes = GetInt();

    // calculate and present result
    int bottles = minutes * 12;
    printf("bottles: %d\n", bottles);

}