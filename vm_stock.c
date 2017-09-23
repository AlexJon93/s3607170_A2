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
		printf("Error: Unable to allocate memory for new list\n");
		return NULL;
	}
}

Node *createNode(Stock *stock)
{
	Node *node;

	if((node = malloc(sizeof(*node))) == NULL)
	{
		printf("Error: Unable to allocate memory for new node\n");
		return NULL;
	}

	if((node->data = malloc(sizeof(*node->data))) == NULL)
	{
		printf("Error: unable to allocate memory for new node data\n");
		free(node);
		return NULL;
	}

	*node->data = *stock;
	node->next = NULL;

	return node;
}

void insertNode(List *list, Node *newNode)
{
	if(list->head == NULL)
	{
		list->head = newNode;
	}

	else
	{
		Node *traverser = list->head;
		Node *prevNode = NULL;

		while(traverser != NULL)
		{
			if(strcmp(newNode->data->name, traverser->data->name) < 0)
				break;
			prevNode = traverser;
			traverser = traverser->next;
		}

		if(prevNode == NULL)
		{
			newNode->next = list->head;
			list-> head = newNode;
		}

		else
		{
			prevNode->next = newNode;
			newNode->next = traverser;
		}
	}

	list->size++;
}