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
{ }

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
	char input[ID_LEN+EXTRA_SPACES];
	Node *traverser = system->itemList->head;
	Stock *item = NULL;

	printf("\nPurchase Item\n");
	printCharacter(strlen("Purchase Item"), CAT_HEAD);
	while(TRUE)
	{
		int dollars;
		int cents;

		printf("\nPlease enter the ID of the item you wish to purchase: \n");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';

		while(traverser != NULL)
		{
			if(strcmp(input, traverser->data->id) == 0)
			{
				item = traverser->data;
			}
			traverser = traverser->next;
		}

		if(item == NULL)
		{
			printf("Error: Invalid ID entered\n");
			continue;
		}

		dollars = item->price.dollars;
		cents = item->price.cents;

		printf("You have selected \"%s - %s\". This will cost $%d.%.2d\n", 
			item->name, item->desc, dollars, cents);

		printf("Please hand over the money - type in the value of each note/coin in cents\n");
		printf("Press enter on a new and empty line to cancel this purchase\n");

		while(TRUE)
		{
			char moneyInput[PRICE_LEN];
			unsigned amount;
			char *ptr;

			printf("You still need to give us $%d.%.2d: ", dollars, cents);
			fgets(moneyInput, sizeof(moneyInput), stdin);
			moneyInput[strcspn(moneyInput, "\n")] = '\0';
			amount = strtoul(moneyInput, &ptr, 10);

			if(amount > 100)
			{
				dollars -= amount / 100;
				amount %= 100;
			}

			if(amount < 100)
			{
				cents -= amount;
				if(cents < 0 && dollars > 0)
				{
					cents += 100;
					dollars--;
				}
			}

			if(dollars == 0 && cents == 0)
			{
				printf("Thank you. Here is your %s.\n", item->name);
				break;
			}

			else if(dollars < 0)
			{
				dollars *= -1;
				if(cents < 0)
					cents *= -1;
				printf("Thank you. Here is your %s, and your change of $%d.%.2d\n", item->name, dollars, cents);
				break;
			}

			else if(dollars == 0 && cents < 0)
			{
				cents *= -1;
				printf("Thank you. Here is your %s, and your change of $%d.%.2d\n", item->name, dollars, cents);
				break;
			}
		}

		break;
	}
}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{
	saveStock(system);
	free(system);
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
