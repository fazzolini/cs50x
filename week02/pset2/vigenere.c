/**
 * caesars.c
 * this is part of CS50X pset2
 *
 * Otto Brut
 * fazzolini@gmail.com
 *
 * version: 1.00
 *
 * Implements a simple Vigenere cypher
 * based on updated Caesar sypher
 * uses the first command-line argument
 * as letter-only key
 */


#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototypes
bool is_valid_character_string(string arg);
bool is_letter(char c);
bool is_lower_case(char c);
bool is_upper_case(char c);
char shift_caesar(char c, int shift);
int get_vigenere_shift(char c);

// hard-coded constants related to ASCII
#define START_LOWER_CASE 97
#define START_UPPER_CASE 65
#define ALPHABET_SIZE 26

int main(int argc, string argv[])
{

	// if no arguments -> error code 1
	if (argc == 1 || argc > 2){
		printf("Your program takes EXACTLY ONE command-line argument.\n");
		return 1;
	}

	// get the key
    string key = argv[1];

    // check if valid key, if invalid -> error code 1
    if (!is_valid_character_string(key))
    {
    	printf("Your command-line argument is INVALID (should be only English letters).\n");
    	return 1;	
    }

	// get user's string input
	printf("plaintext:");
	string s = GetString();

	// perform conversion itself
	printf("ciphertext:");
	for (int i = 0, n = strlen(s), j = 0; i < n; i++)
	{
		char original_char = s[i];
		int key_position = j % strlen(key);
		int shift = get_vigenere_shift(key[key_position]);
		char new_char = shift_caesar(original_char, shift);
		printf("%c", new_char);
		// need j counter to correctly 'spread' key around letters
		// should ignore all non-letter characters
		if (is_letter(s[i])) j++;
	}

	// print the new line at the end
	printf("\n");

	return 0;
}


/**
 * this function checks if arg is a
 * valid string, i.e.
 * only characters
 */
bool is_valid_character_string(string arg)
{
	for (int i = 0, n = strlen(arg); i < n; i++)
	{
		if (!is_letter(arg[i]))
		{
			return false;
		}
	}
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
	if (c < START_LOWER_CASE || c > (START_LOWER_CASE + ALPHABET_SIZE - 1))
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
	if (c < START_UPPER_CASE || c > (START_UPPER_CASE + ALPHABET_SIZE - 1))
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
			lower_range = START_LOWER_CASE;
		} else
		{
			lower_range = START_UPPER_CASE;
		}

		// perform the shift itself
		char new_char = lower_range + (c - lower_range + shift) % ALPHABET_SIZE;
		return new_char;
	} else
	{
		// if not letter return original char
		return c;
	}
}


/**
 * this function returns
 * vigenere shift size
 * based on a char c
 * only gives a shift if char
 * is a letter
 * if not, shift is 0
 */
int get_vigenere_shift(char c)
{
	if (is_letter(c))
	{
		int lower_range;
		if (is_lower_case(c))
		{
			lower_range = START_LOWER_CASE;
		} else
		{
			lower_range = START_UPPER_CASE;
		}
		return (int)(c - lower_range);
	} else {
		return 0;
	}
}