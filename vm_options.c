/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/

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
Boolean loadData(VmSystem * system, const char * stockFileName, const char * coinsFileName)
{
	validateInputFiles(stockFileName, coinsFileName);
	if(!loadStock(system, stockFileName))
		return FALSE;
	else
		system->stockFileName = stockFileName;

	if(coinsFileName != NULL)
	{
		if(!loadCoins(system, coinsFileName))
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
			if(!isalpha(stock.id[0]))
			{
				printf("Error: id in stock file is invalid\n");
				return FALSE;
			}
			if(!checkStringNum(memmove(&stock.name[0], &stock.name[1], strlen(stock.name) - 1)))
			{
				printf("Error: id in stockFile is invalid\n");
				return FALSE;
			}

			strcpy(stock.name, strtok(NULL, STOCK_DELIM));
			if(!checkStringAlpha(stock.name))
			{
				printf("Error: name in stock file is invalid\n");
				return FALSE;
			}
			strcpy(stock.desc, strtok(NULL, STOCK_DELIM));
			if(!checkStringAlpha(stock.desc))
			{
				printf("Error: description in stock file is invalid\n");
				return FALSE;
			}
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

	printf("Error: Unable to open stock file.\n");
    return FALSE;
}

/**
 * Loads the coin file data into the system.
 **/
Boolean loadCoins(VmSystem * system, const char * fileName)
{
	FILE *coinsFile;
	char line[MAX_LEN];
	Coin *coins = system->cashRegister;

	if((coinsFile = fopen(fileName, "r")) != NULL)
	{
		while(fgets(line, sizeof line, coinsFile) != NULL)
		{
			Denomination denom;
			char *ptr;

			if((denom = convertCoinToDenom(strtoul(strtok(line, COIN_DELIM), &ptr, 10))) < 0)
			{
				printf("Error: coins file contains invalid coin denomination\n");
				return FALSE;
			}

			coins[denom].count = strtoul(strtok(NULL, END_DELIM), &ptr, 10);
		}

		return TRUE;
	}

	printf("Error: Unable to open coins file.\n");
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

			fprintf(stockFile, "%s|%s|%s|%d.%.2d|%d\n",
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
	FILE *coinsFile;

	if((coinsFile = fopen(system->coinFileName, "w")) != NULL)
	{
		int i;
		for(i = 0; i < NUM_DENOMS; i++)
		{
			fprintf(coinsFile, "%d,%d\n", denomToCents(system->cashRegister[i].denom), system->cashRegister[i].count);
		}
		fclose(coinsFile);
		return TRUE;
	}

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
	Stock *stock = NULL;
	int total, inputted;

	printf("Purchase Item\n");
	printCharacter(strlen("Purchase Item"), '-');
	printf("\nPlease enter the id of the item you wish to purchase: ");

	fgets(id, sizeof id, stdin);
	if(strcmp(id, END_DELIM) == 0)
		return;
	strtok(id, END_DELIM);
	stock = getByID(system->itemList, id);

	if(stock == NULL)
	{
		printf("Error: not valid item id\n");
		purchaseItem(system);
	}

	printf("You have selected \"%s %s\". This will cost you $%u.%.2u.\n",
	stock->name, stock->desc, stock->price.dollars, stock->price.cents);
	printf("Please hand over the money - type in the value of each note/coin in cents.\n"
		"Press enter on a new and empty line to cancel this purchase:\n");

	inputted = 0;
	total = stock->price.dollars * 100 + stock->price.cents;
	while(total > 0)
	{
		char input[PRICE_LEN + EXTRA_SPACES];
		char *ptr;
		int current;

		printf("You still need to give us %u.%.2u: ", total / 100, total % 100);
		fgets(input, sizeof input, stdin);
		if(strcmp(input, END_DELIM) == 0)
		{
			if(inputted != 0)
			{
				printf("Purchase cancelled, here is your change: ");
				printChange(system, inputted);
			}
			return;
		}
		current = strtoul(strtok(input, END_DELIM), &ptr, 10);
		if(!checkDenom(current))
		{
			printf("Error: %u.%.2u not valid denomination of money\n", current / 100, current % 100);
			continue;
		}

		inputted += current;
		total -= current;
	}

	if(total == 0)
		printf("Thank you here is your %s.", stock->name);
	else
	{
		int remainder = total * -1;
		printf("Thank you here is your %s, and your change of %u.%.2u:", stock->name, remainder / 100, remainder % 100);
		printChange(system, remainder);
	}

	printf("\nPlease come back soon.\n");
}

/**
 * You must save all data to the data files that were provided on the command
 * line when the program loaded up, display goodbye and free the system.
 * This function implements requirement 6 of the assignment specification.
 **/
void saveAndExit(VmSystem * system)
{
	saveStock(system);
	saveCoins(system);
	systemFree(system);
	printf("\nGoodbye. \n\n");
}

/**
 * This option adds an item to the system. This function implements
 * requirement 7 of of assignment specification.
 **/
void addItem(VmSystem * system)
{ 
	char *ptr;
	Stock stock;
	Node *node;

	char id[ID_LEN + NULL_SPACE];
	char name[NAME_LEN + NULL_SPACE];
	char desc[DESC_LEN + NULL_SPACE];
	char price[PRICE_LEN + NULL_SPACE];

	sprintf(id, "I%.4d", system->itemList->size+1);
	printf("This new item will have the Item id of %s\n", id);
	strcpy(stock.id, id);
	printf("Enter the item name: ");
	fgets(name, sizeof name, stdin);
	if(strcmp(name, END_DELIM) == 0)
		return;
	strcpy(stock.name, strtok(name, END_DELIM));
	printf("Enter the item description: ");
	fgets(desc, sizeof desc, stdin);
	if(strcmp(desc, END_DELIM) == 0)
		return;
	strcpy(stock.desc, strtok(desc, END_DELIM));
	printf("Enter the price for this item: ");
	fgets(price, sizeof price, stdin);
	if(strcmp(price, END_DELIM) == 0)
		return;

	stock.price.dollars = strtoul(strtok(price, PRICE_DELIM), &ptr, 10);
	stock.price.cents = strtoul(strtok(NULL, END_DELIM), &ptr, 10);
	stock.onHand = DEFAULT_STOCK_LEVEL;

	if((node = createNode(&stock)) == NULL)
		return;
	insertNode(system->itemList, node);
}

/**
 * Remove an item from the system, including free'ing its memory.
 * This function implements requirement 8 of the assignment specification.
 **/
void removeItem(VmSystem * system)
{
	char id[ID_LEN + EXTRA_SPACES];
	printf("Enter the item id of the item to remove from the menu:");
	fgets(id, sizeof id, stdin);
	if(strcmp(id, END_DELIM) == 0)
		return;
	strtok(id, END_DELIM);
	if(!removeNode(system->itemList, id))
		printf("Error: not a valid id\n");
}

/**
 * This option will require you to display the coins from lowest to highest
 * value and the counts of coins should be correctly aligned.
 * This function implements part 4 of requirement 18 in the assignment
 * specifications.
 **/
void displayCoins(VmSystem * system)
{
	int i;
	printf("\nCoins Summary\n");
	printCharacter(20, '-');
	printf("\nDenomination\t | Count\n\n");
	for(i = 0; i < NUM_DENOMS; i++) 
	{
		printf("%s  \t | %d\n", printDenom(system, i), system->cashRegister[i].count);
	}
}

/**
 * This option will require you to iterate over every stock in the
 * list and set its onHand count to the default value specified in
 * the startup code.
 * This function implements requirement 9 of the assignment specification.
 **/
void resetStock(VmSystem * system)
{ 
	Node *traverser = system->itemList->head;

	while(traverser != NULL)
	{
		traverser->data->onHand = DEFAULT_STOCK_LEVEL;
		traverser = traverser->next;
	}
}

/**
 * This option will require you to iterate over every coin in the coin
 * list and set its 'count' to the default value specified in the
 * startup code.
 * This requirement implements part 3 of requirement 18 in the
 * assignment specifications.
 **/
void resetCoins(VmSystem * system)
{
	int i;
	for(i = 0; i < NUM_DENOMS; i++) 
	{
		system->cashRegister[i].count = DEFAULT_COIN_COUNT;
	}
}

/**
 * This option will require you to display goodbye and free the system.
 * This function implements requirement 10 of the assignment specification.
 **/
void abortProgram(VmSystem * system)
{
	systemFree(system);
	printf("\nGoodbye. \n\n");
}
