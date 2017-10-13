/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/

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

Boolean validateInputFiles(const char * stockFileName, const char * coinsFileName)
{
    FILE *stockFile, *coinsFile;
    char line[MAX_LEN];

    if((stockFile = fopen(stockFileName, "r")) != NULL)
    {
        while(fgets(line, sizeof line, stockFile) != NULL)
        {
            if(countCharOccur(line, '|') > 4)
            {
                printf("Error: stock file contains too many fields\n");
                return FALSE;
            }
            else if (countCharOccur(line, '|') < 4)
            {
                printf("Error: stock file contains too few fields\n");
                return FALSE;
            }
        }
    }
    else
        return FALSE;
    if((coinsFile = fopen(coinsFileName, "r")) != NULL)
    {
        while(fgets(line, sizeof line, coinsFile) != NULL)
        {
            if(countCharOccur(line, ',') > 1)
            {
                printf("Error: coins file contains too many fields\n");
                return FALSE;
            }
            else if (countCharOccur(line, '|') < 1)
            {
                printf("Error: stock file contains too few fields\n");
                return FALSE;
            }
        }
    }

    return FALSE;
}

int countCharOccur(char *line, char check)
{
    int total = 0, i = 0;

    while(line[i] != '\n')
    {
        if(line[i] == check)
            total++;
        i++;
    }

    return total;
}

Boolean checkStringAlpha(char *string)
{
    int i = 0;

    while(string[i] != '\0')
    {
        if(!isalpha(string[i]))
            return FALSE;
        i++;
    }

    return TRUE;
}

Boolean checkStringNum(char *string)
{
    int i = 0;

    while(string[i] != '\0')
    {
        if(!isdigit(string[i]))
            return FALSE;
        i++;
    }

    return TRUE;
}