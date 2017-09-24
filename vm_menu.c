#include "vm_menu.h"

/**
 * vm_menu.c handles the initialisation and management of the menu array.
 **/

/**
 * In this function you need to initialise the array of menu items
 * according to the text to be displayed for the menu. This array is
 * an array of MenuItem with text and a pointer to the function
 * that will be called.
 **/
void initMenu(MenuItem * menu)
{
	strcpy(menu[0].text, "1. Display Items");
	strcpy(menu[1].text, "2. Purchase Items");
	strcpy(menu[2].text, "3. Save and Exit");
	strcpy(menu[3].text, "4. Add Item");
	strcpy(menu[4].text, "5. Remove Item");
	strcpy(menu[5].text, "6. Display Coins");
	strcpy(menu[6].text, "7. Reset Stock");
	strcpy(menu[7].text, "8. Reset Coins");
	strcpy(menu[8].text, "9. Abort Program");

	menu[0].function = &displayItems;
	menu[1].function = &purchaseItem;
	menu[2].function = &saveAndExit;
	menu[3].function = &addItem;
	menu[4].function = &removeItem;
	menu[5].function = &displayCoins;
	menu[6].function = &resetStock;
	menu[7].function = &resetCoins;
	menu[8].function = &abortProgram;
}

/**
 * Gets input from the user and returns a pointer to the MenuFunction
 * that defines how to perform the user's selection. NULL is returned
 * if an invalid option is entered.
 **/
MenuFunction getMenuChoice(MenuItem * menu)
{
	int iterate = 0;
	char input[INPUT_SIZE];
	char *ptr;

	for(iterate = 0; iterate < MENU_ITEM_NO; iterate++)
	{
		if(iterate == 0)
			printf("\nMain Menu:\n");
		if(iterate == 4)
			printf("Administrator-Only Menu:\n");
		printf(" %s\n", menu[iterate].text);
	}
	printf("Select your option (1-9):\n ");

	fgets(input, sizeof(input), stdin);
	if(isdigit(input[0]))
		return menu[strtoul(input, &ptr, 10) - 1].function;
	

    return NULL;
}