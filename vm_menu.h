#ifndef VM_MENU_H
#define VM_MENU_H

#include "vm_options.h"

/**
 * The maximum length of a menu item's text.
 **/
#define MENU_NAME_LEN 50

/**
 * The number of menu items to be in the menu array
 **/

#define MENU_ITEM_NO 9

/**
 * Maximum size of user input for menu item selection
 **/
#define INPUT_SIZE (3 + EXTRA_SPACES)

/**
 * Represents a function that can be selected from the list of
 * menu_functions - creates a new type called a menu_function.
 */
typedef void (*MenuFunction)(VmSystem *);

/**
 * Represents a menu item to be displayed and executed in the program.
 **/
typedef struct menu_item
{
    char text[MENU_NAME_LEN + NULL_SPACE];
    MenuFunction function;
} MenuItem;

void initMenu(MenuItem * menu);
MenuFunction getMenuChoice(MenuItem * menu);

#endif
