#include <cs50.h>
#include <stdio.h>

int main(void)
{
	// first deal with input
	printf("O hai! ");
	float raw_input;
	do
	{
		printf("How much change is owed?\n");
		raw_input = GetFloat();
	}
	while(raw_input < 0.0);

    /*
    ** now it gets UGLY
    ** this is to deal with 4.2 * 100 = 419.999
    ** SERISLY, WAT DA FUK, C?!
    */
    int thousands = (int)(raw_input * 1000);
    int cents = thousands / 10;
    int remainder = thousands % 10;
    if (remainder == 9) cents++;

	// now the algo itself
	// while there is change
	int result = 0;
	while(cents)
	{
		if (cents >= 25)
		{
			cents -= 25;
			result++;
		} else if (cents >= 10)
		{
			cents -= 10;
			result++;
		} else if (cents >= 5)
		{
			cents -= 5;
			result++;
		} else if (cents >= 1)
		{
			cents -= 1;
			result++;
		}
	}

	// produce result to the user on screen
	printf("%d\n", result);
}