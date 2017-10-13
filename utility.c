#include "utility.h"

/**
 * Function required to be used when clearing the buffer. It simply reads
 * each char from the buffer until the buffer is empty again. Please refer
 * to the materials on string and buffer handling in the course for more
 * information.
 **/
void readRestOfLine()
{
    int ch;
    while(ch = getc(stdin), ch != EOF && ch != '\n')
    { } /* Gobble each character. */

    /* Reset the error status of the stream. */
    clearerr(stdin);
}

/**
 * Prints a specified number of received character, used for formatting.
 **/
void printCharacter(int amount, char print)
{
	int counter;

	for(counter = 0; counter < amount; counter++)
		printf("%c", print);
}

/**
 * Counts the number of digits in an int, used for formatting
 **/
int countDigits(int n) 
{
    if (n < 10) return 1;
    return 1 + countDigits(n / 10);
}