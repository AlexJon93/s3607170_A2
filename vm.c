#include "vm.h"

int main(int argc, char ** argv)
{
	VmSystem system;
    MenuItem menu[MENU_ITEM_NO];
    MenuFunction inputFunc;

    if(argc != STK_ARG && argc != CON_ARG)
    {
        printf("Invalid number of args\n\n");
        return EXIT_FAILURE;
    }

    if(!systemInit(&system))
    	return EXIT_FAILURE;
    if(!loadData(&system, argv[STK_ARG-1], argv[CON_ARG-1]))
    	return EXIT_FAILURE;
    initMenu(menu);

    do{

        inputFunc = getMenuChoice(menu);
        inputFunc(&system);

    } while(inputFunc != &saveAndExit && inputFunc != &abortProgram);

    printf("\nGoodbye. \n\n");

    return EXIT_SUCCESS;
}