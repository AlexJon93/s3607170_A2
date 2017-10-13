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
	List *list = malloc(sizeof(list + 1));


	if(list != NULL)
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

void freeList(List *list)
{
	Node *traverser = list->head;
	Node *temp;

	while(traverser != NULL)
	{
		temp = traverser;
		traverser = traverser->next;
		freeNode(temp);
	}

	free(list);
}

void freeNode(Node *node)
{
	free(node->data);
	free(node);
}

/**
 * This function returns an array with the largest character size for each
 * piece of data to be displayed in the displayItems function
 **/
void getColumnSizes(List *list, int *sizes)
{
	Node *traverser = list->head;

	while(traverser != NULL)
	{
		sizes[0] = strlen(traverser->data->id);
		if(strlen(traverser->data->name) > sizes[1])
			sizes[1] = strlen(traverser->data->name);
		if(countDigits(traverser->data->price.dollars) > sizes[2])
			sizes[2] = countDigits(traverser->data->price.dollars) + 5;
		traverser = traverser->next;
	}
}

/**
 * This function sorts the received linkedlist in order of the value of each node's id string
 **/
void sortByID(List *list)
{
	int i;

	for(i = 0; i < list->size; i++)
	{
		Node *nextNode = list->head->next;
		Node *currNode = list->head;
		Node *prevNode = NULL;

		while(nextNode != NULL)
		{
			if(strcmp(currNode->data->id, nextNode->data->id) > 0)
			{
				Node *temp;

				currNode->next = nextNode->next;
				nextNode->next = currNode;
				if(prevNode != NULL)
					prevNode->next = nextNode;
				else
					list->head = nextNode;

				temp = currNode;
				currNode = nextNode;
				nextNode = temp;				
			}

			prevNode = currNode;
			currNode = currNode->next;
			nextNode = nextNode->next;
		}
	}
}