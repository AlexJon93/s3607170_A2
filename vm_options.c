#include "vm_options.h"

/**
 * vm_options.c this is where you need to implement the system handling
 * functions (e.g., init, free, load, save) and the main options for
 * your program. You may however have some of the actual work done
 * in functions defined elsewhere.
 **/

/**
 * Initialise the system to a known safe state. Look at the structure
 * defined in vm_system.h.
 **/
Boolean systemInit(VmSystem * system)
{
	if((system->itemList = createList()) != NULL)
	{
		system->itemList = createList();
		initCoinArr(system->cashRegister);
		system->stockFileName = NULL;
		system->coinFileName = NULL;

		return TRUE;
	}

    return FALSE;
}

/**
 * Free all memory that has been allocated. If you are struggling to
 * find all your memory leaks, compile your program with the -g flag
 * and run it through valgrind.
 **/
void systemFree(VmSystem * system)
{
	freeList(system->itemList);
}

/**
 * Loads the stock and coin data into the system. You will also need to assign
 * the char pointers to the stock and coin file names in the system here so
 * that the same files will be used for saving. A key part of this function is
 * validation. A substantial number of marks are allocated to this function.
 **/
Boolean loadData(
    VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
	if(loadStock(system, stockFileName) == FALSE)
		return FALSE;
	else
		system->stockFileName = stockFileName;

	if(coinsFileName != NULL)
	{
		if(loadCoins(system, coinsFileName) == FALSE)
			return FALSE;
		else
			system->coinFileName = coinsFileName;
	}

    return TRUE;
}

/**
 * Loads the stock file data into the system.
 **/
Boolean loadStock(VmSystem * system, const char * fileName)
{
	FILE *stockFile;
	char line[MAX_LEN];

	if((stockFile = fopen(fileName, "r")) != NULL)
	{
		while(fgets(line, sizeof line, stockFile) != NULL)
		{
			Stock stock;
			Node *node;
			char *ptr;

			strcpy(stock.id, strtok(line, STOCK_DELIM));
			strcpy(stock.name, strtok(NULL, STOCK_DELIM));
			strcpy(stock.desc, strtok(NULL, STOCK_DELIM));
			stock.price.dollars = strtoul(strtok(NULL, PRICE_DELIM), &ptr, 10);
			stock.price.cents = strtoul(strtok(NULL, STOCK_DELIM), &ptr, 10);
			stock.onHand = strtoul(strtok(NULL, END_DELIM), &ptr, 10);

			if((node = createNode(&stock)) == NULL)
				return FALSE;

			insertNode(system->itemList, node);
		}

		fclose(stockFile);
		return TRUE;
	}

    return FALSE;
}

/**
 * Loads the coin file data into the system.
 **/
Boolean loadCoins(VmSystem * system, const char * fileName)
{
    return FALSE;
}

/**
 * Saves all the stock back to the stock file.
 **/
Boolean saveStock(VmSystem * system)
{
	FILE *stockFile;

	sortByID(system->itemList);
	if((stockFile = fopen(system->stockFileName, "w")) != NULL)
	{
		Node *traverser = system->itemList->head;

		while(traverser != NULL)
		{
			Stock *stock = traverser->data;

			fprintf(stockFile, "%s|%s|%s|%d.%d|%d\n",
				stock->id,
				stock->name,
				stock->desc,
				stock->price.dollars,
				stock->price.cents,
				stock->onHand);

			traverser = traverser->next;
		}
		fclose(stockFile);
		return TRUE;
	}

    return FALSE;
}

/**
 * Saves all the coins back to the coins file.
 **/
Boolean saveCoins(VmSystem * system)
{
    return FALSE;
}

/**
 * This option allows the user to display the items in the system.
 * This is the data loaded into the linked list in the requirement 2.
 **/
void displayItems(VmSystem * system)
{
	List *list = system->itemList;
	Node *traverser = list->head;
	int sizes[DATA_CAT_NO] = {0};

	getColumnSizes(list, sizes);

	printf("\nItems Menu\n\n");

	printf("%-*s", sizes[0], "ID");
	printf("%s%-*s",CAT_SEP, sizes[1], "Name");
	printf("%sAvailable", CAT_SEP);
	printf("%sPrice", CAT_SEP);
	printf("\n");

	printCharacter(
		sizes[0]+strlen(CAT_SEP)+sizes[1]+strlen(CAT_SEP)+strlen("Available")+strlen(CAT_SEP)+sizes[2], 
		CAT_HEAD
		);
	printf("\n");

	while(traverser != NULL)
	{
		printf("%-*s", sizes[0], traverser->data->id);
		printf("%s%-*s", CAT_SEP, sizes[1], traverser->data->name);
		printf("%s%-9u", CAT_SEP, traverser->data->onHand);
		printf("%s$ %u.%.2u", CAT_SEP, traverser->data->price.dollars, traverser->data->price.cents);
		printf("\n");

		traverser = traverser->next;
	}
}

/**
 * This option allows the user to purchase an item.
 * This function implements requirement 5 of the assignment specification.
 **/
void purchaseItem(VmSystem * system)
{
	char id[ID_LEN + EXTRA_SPACES];
	List *list = system->itemList;
	Node *traverser = list->head;
	Stock *stock = NULL;
	int dollars, cents, total;

	printf("Purchase Item\n");
	printCharacter(strlen("Purchase Item"), '-');
	printf("\nPlease enter the id of the item you wish to purchase: ");
	fgets(id, sizeof id, stdin);

	id[strlen(id)-1] = '\0';

	while(traverser != NULL)
	{
		if(strcmp(traverser->data->id, id) == 0)
		{
			stock = traverser->data;
			break;
		}

		traverser = traverser->next;
	}

	if(stock == NULL)
	{
		printf("Error: not valid item id\n");
		purchaseItem(system);
		return;
	}

	printf("You have selected \"%s %s\". This will cost you $%u.%.2u.\n",
	stock->name, stock->desc, stock->price.dollars, stock->price.cents);
	printf("Please hand over the money - type in the value of each note/coin in cents.\n"
		"Press enter on a new and empty line to cancel this purchase:\n");

	total = (stock->price.dollars * 100) + stock->price.cents;

	while(total > 0)
	{
		char *ptr;
		char input[PRICE_LEN];
		int deposited;

		dollars = total / 100;
		cents = total % 100;

		printf("You still need to give us $%u.%.2u: ", dollars, cents);
		deposited = strtoul(fgets(input, sizeof input, stdin), &ptr, 10);
		total -= deposited;
	}

	total *= -1;
	dollars = total / 100;
	cents = total % 100;

	if(total == 0)
		printf("Thank you. Here is your %s.\n", stock->name);
	else
		printf("Thank you. Here is your %s, and your change of $%u.%.2u\n", 
			stock->name, dollars, cents);
	printf("Please come back soon.\n");
	
}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{
	saveStock(system);
	systemFree(system);
}

/**
 * This option adds an item to the system. This function implements
 * requirement 7 of of assignment specification.
 **/
void addItem(VmSystem * system)
{ printf("Not yet implemented\n"); }

/**
 * Remove an item from the system, including free'ing its memory.
 * This function implements requirement 8 of the assignment specification.
 **/
void removeItem(VmSystem * system)
{ printf("Not yet implemented\n"); }

/**
 * This option will require you to display the coins from lowest to highest
 * value and the counts of coins should be correctly aligned.
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
void displayCoins(VmSystem * system)
{ printf("Not yet implemented\n"); }

/**
 * This option will require you to iterate over every stock in the
 * list and set its onHand count to the default value specified in
 * the startup code.
 * This function implements requirement 9 of the assignment specification.
 **/
void resetStock(VmSystem * system)
{ printf("Not yet implemented\n"); }

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code.
 * This requirement implements part 3 of requirement 18 in the
 * assignment specifications.
 **/
void resetCoins(VmSystem * system)
{ printf("Not yet implemented\n"); }

/**
 * This option will require you to display goodbye and free the system.
 * This function implements requirement 10 of the assignment specification.
 **/
void abortProgram(VmSystem * system)
{ printf("Not yet implemented\n"); }
