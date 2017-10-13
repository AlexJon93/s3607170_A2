/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/

#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>

/**
 * The maximum length of string line in a file
 **/
#define MAX_LEN 315

typedef enum boolean
{
    FALSE = 0,
    TRUE
} Boolean;

#define NEW_LINE_SPACE 1
#define NULL_SPACE 1

/**
 * This is used to compensate for the extra character spaces taken up by
 * the '\n' and '\0' when input is read through fgets().
 **/
#define EXTRA_SPACES (NEW_LINE_SPACE + NULL_SPACE)

#define EMPTY_STRING ""
#define END_DELIM "\n"

/**
 * Call this function whenever you detect buffer overflow.
 **/
void readRestOfLine();
/**
 * Formatting related functions
 **/
void printCharacter(int amount, char print);
int countDigits(int n);
/*
 * Checks both input files for any issues, completes requirement 19
 */
Boolean validateInputFiles(const char * stockFileName, const char * coinsFileName);
int countCharOccur(char *line, char check);
Boolean checkStringAlpha(char *string);
Boolean checkStringNum(char *string);
#endif
