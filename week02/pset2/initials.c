/**
 * initials.c
 * this is part of CS50X pset2
 *
 * Otto Brut
 * fazzolini@gmail.com
 *
 * version: 1.00
 *
 * Get an input name and prints initials of person
 * (uses only spaces as separator)
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
	// get input from user
	string name = GetString();

	// status to know if first letter
	bool is_first_letter = true;

	// loop and collect output
	for (int i = 0, n = strlen(name); i < n; i++)
	{
		// can deal with multiple spaces
		if (is_first_letter && name[i] != ' ')
		{
			printf("%c", toupper(name[i]));
			is_first_letter = false;
		}
		// space -> next letter is first letter
		if (name[i] == ' ')
		{
			is_first_letter = true;
		}
		// otherwise do nothing
	}
	
	// new line at the end
	printf("\n");
}