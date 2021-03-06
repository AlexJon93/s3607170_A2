/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/
#ifndef VM_STOCK_H
#define VM_STOCK_H

#include "vm_coin.h"

/**
 * The default stock level that all new stock should start with and the value
 * to use when restting stock.
 **/
#define DEFAULT_STOCK_LEVEL 20

#define STOCK_DELIM "|"
#define PRICE_DELIM "."

/**
 * This is the number of data categories displayed in the displayItem function
 **/
#define DATA_CAT_NO 3

/**
 * Used to seperate data categories and headings in displayItem function respectively
 **/
#define CAT_SEP " | "
#define CAT_HEAD '-'

List *createList();
Node *createNode(Stock *stock);
void insertNode(List *list, Node *newNode);
Boolean removeNode(List *list, char *id);
void freeList(List *list);
void freeNode(Node *node);
void getColumnSizes(List *list, int *sizes);
void sortByID(List *list);
Stock *getByID(List *list, char *id);

#endif
