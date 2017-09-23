#include "vm_stock.h"

/**
 * vm_stock.c this is the file where you will implement the
 * interface functions for managing the stock list.
 **/

/**
 * Some example functions:
 * create list, free list, create node, free node, insert node, etc...
 */

List *createList()
{
	List *list;


	if((list = malloc(sizeof(list))) != NULL)
	{
		list->head = NULL;
		list->size = 0;

		return list;
	}

	else
	{
		printf("Unable to allocate memory");
		return NULL;
	}
}