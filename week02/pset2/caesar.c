/**
 * caesars.c
 * this is part of CS50X pset2
 *
 * Otto Brut
 * fazzolini@gmail.com
 *
 * version: 1.00
 *
 * Implements a simple Caesar cypher
 * uses the first command-line argument
 * as non-negative integer shift
 */


#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototypes
bool is_valid_integer_string(string arg);
bool is_letter(char c);
bool is_lower_case(char c);
bool is_upper_case(char c);
char shift_caesar(char c, int shift);

int main(int argc, string argv[])
{

	// if no arguments -> error code 1
	if (argc == 1 || argc > 2){
		printf("Your program takes EXACTLY ONE integer command-line argument.\n");
		return 1;
	}

	// convert command-line argument to integer
    int shift;
	if (is_valid_integer_string(argv[1]))
	{
	    shift = atoi(argv[1]);
	} else 
	{
		printf("Your command-line argument was not in an integer-convertible form.\n");
		return 2;
	}

	// get user's string input
	string s = GetString();

	// perform conversion itself
	for (int i = 0, n = strlen(s); i < n; i++)
	{
		char original_char = s[i];
		char new_char = shift_caesar(original_char, shift);
		printf("%c", new_char);
	}

	// print the new line at the end
	printf("\n");

	return 0;
}


/**
 * this function checks if arg is a
 * valid string, i.e.
 * only digits
 */
bool is_valid_integer_string(string arg)
{
	// first can be from 1-9
	if ((arg[0] < '1') || (arg[0] > '9'))
	{
		// return false
		printf("Invalid argument!\n");
		return false;
	}
	// others can be 0-9
	for (int i = 1, n = strlen(arg); i < n; i++)
	{
		if ((arg[i] < '0') || (arg[i] > '9'))
		{
			// return false
			printf("Invalid argument!\n");
			return false;
		}
	}
	// can return a valid argument!
	return true;
}


/**
 * this function checks if char
 * is an ASCII letter
 */
bool is_letter(char c)
{
	if (is_lower_case(c) || is_upper_case(c))
	{
		return true;
	}
	return false;
}


/**
 * this function checks if char
 * is a lower case letter
 */
bool is_lower_case(char c)
{
	if (c < 97 || c > 122)
	{
		return false;
	}
	return true;
}


/**
 * this function checks if char
 * is an upper case letter
 */
bool is_upper_case(char c)
{
	if (c < 65 || c > 90)
	{
		return false;
	}
	return true;
}


/**
 * this function shifts a char
 * by shift steps
 * depending on whether it is
 * lower or upper case
 *
 * returns shifted char
 */
char shift_caesar(char c, int shift)
{
	// if it is a letter
	if (is_letter(c))
	{
		// declaration for lower and upper range for letters
		int lower_range;

		// assign appropriate lower range
		if (is_lower_case(c))
		{
			lower_range = 97;
		} else
		{
			lower_range = 65;
		}

		// perform the shift itself
		char new_char = lower_range + (c - lower_range + shift) % 26;
		return new_char;
	} else
	{
		// if not letter return original char
		return c;
	}
}