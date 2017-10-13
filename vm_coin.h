/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/
#ifndef VM_COIN_H
#define VM_COIN_H

#include "vm_system.h"

/**
 * The default coin level used when resetting values.
 **/
#define DEFAULT_COIN_COUNT 20

#define COIN_DELIM ","

void initCoinArr(Coin *coins);
Denomination convertCoinToDenom(unsigned value);
Boolean checkDenom(int value);
Boolean deductDenom(Denomination denom, Price *price);
unsigned denomToCents(Denomination denom);
void printChange(VmSystem *system, int change);
char *printDenom(VmSystem *system, int index);

#endif
