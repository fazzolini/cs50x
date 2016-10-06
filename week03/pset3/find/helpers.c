/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
int findMidPoint(int min, int max)
{
    return (min + max) / 2;
}

/**
 * Non-recursive binary search
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int min = 0;
    int max = n - 1;
    int midpoint;
    while (min <= max)
    {
        midpoint = findMidPoint(min, max);
        
        if (value < values[midpoint])
        {
            // go LEFT (modify max)
            max = midpoint - 1;
        }
        else if (values[midpoint] < value)
        {
            // go RIGHT (modify min)
            min = midpoint + 1;
        }
        else
        {
            // found element
            return true;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 * NB!: arrays are passed by reference
 * and not by value (copy)
 */
void sort(int values[], int n)
{
    // implementing a bubble sort
    int swapped = -1;
    while (swapped != 0)
    {
        // re-assign swapped to 0
        swapped = 0;
        // pass-through array performing swaps
        // go up to the element before last
        for (int i = 0; i < n - 1; i++)
        {
            // swap if out of order
            if (values[i] > values[i+1])
            {
                int temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
                swapped += 1;
            }
        }
    }
    return;
}