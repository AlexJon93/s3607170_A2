/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/
#include "vm.h"

int main(int argc, char ** argv)
{
	VmSystem system;
    MenuItem menu[MENU_ITEM_NO];
    MenuFunction inputFunc;

    if(argc != ARG_NO)
    {
        printf("Invalid number of args\n\n");
        return EXIT_FAILURE;
    }

    if(!systemInit(&system))
    	return EXIT_FAILURE;
    if(!loadData(&system, argv[1], argv[2]))
    	return EXIT_FAILURE;
    initMenu(menu);

    do{

        inputFunc = getMenuChoice(menu);
        inputFunc(&system);

    } while(inputFunc != &saveAndExit && inputFunc != &abortProgram);

    return EXIT_SUCCESS;
}