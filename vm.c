#include "vm.h"

int main(int argc, char ** argv)
{
    if(argc != ARG_NUM)
    {
        printf("Invalid number of args\n\n");
        return EXIT_FAILURE;
    }

    printf("Goodbye. \n\n");

    return EXIT_SUCCESS;
}
