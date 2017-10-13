/******************************************************************************

** Student Name: Alexander Jarvis

** Student Number: s3607170

** Date of submission: Fri, 13 Oct 2017

** Course: COSC1076, Semester 2, 2017

******************************************************************************/
#include "vm_coin.h"

/**
 * Implement functions here for managing coins and the
 * "cash register" contained in the VmSystem struct.
 **/

/**
 * Some example functions:
 * init coins array, insert coin, change coin count for Denomination,
 * convert Denomination to coin value, deducting coins from register, etc...
 */

void initCoinArr(Coin *coins)
{
	int i;

	coins[FIVE_CENTS].denom = FIVE_CENTS;
	coins[TEN_CENTS].denom = TEN_CENTS;
	coins[TWENTY_CENTS].denom = TWENTY_CENTS;
	coins[FIFTY_CENTS].denom = FIFTY_CENTS;
	coins[ONE_DOLLAR].denom = ONE_DOLLAR;
	coins[TWO_DOLLARS].denom = TWO_DOLLARS;
	coins[FIVE_DOLLARS].denom = FIVE_DOLLARS;
	coins[TEN_DOLLARS].denom = TEN_DOLLARS;

	for(i = 0; i < NUM_DENOMS; i++)
	{
		coins[i].count = 0;
	}
}

Denomination convertCoinToDenom(unsigned value)
{
	switch(value)
	{
		case 1000:
			return TEN_DOLLARS;
		case 500:
			return FIVE_DOLLARS;
		case 200:
			return TWO_DOLLARS;
		case 100:
			return ONE_DOLLAR;
		case 50:
			return FIFTY_CENTS;
		case 20:
			return TWENTY_CENTS;
		case 10:
			return TEN_CENTS;
		case 5:
			return FIVE_CENTS;
		default:
			return -1;
	}
}

Boolean checkDenom(int value)
{
	switch(value)
	{
		case 1000:
		case 500:
		case 200:
			return TRUE;
		case 100:
		case 50:
		case 20:
		case 10:
		case 5:
			return TRUE;
		default:
			return FALSE;
	}
}

unsigned denomToCents(Denomination denom)
{
	switch(denom) 
	{
	    case TEN_DOLLARS:
            return 1000;
    	case FIVE_DOLLARS:
            return 500;
        case TWO_DOLLARS:
            return 200;
        case ONE_DOLLAR:
            return 100;
        case FIFTY_CENTS:
            return 50;
        case TWENTY_CENTS:
            return 20;
        case TEN_CENTS:
            return 10;
        case FIVE_CENTS:
            return 5;
        default:
            return 0;
    }
}

void printChange(VmSystem *system, int change)
{
	Coin *coinregister = system->cashRegister;

	while(change > 0)
	{
		if(change >= 1000 && coinregister[TEN_DOLLARS].count > 0)
		{
			change -= 1000;
			coinregister[TEN_DOLLARS].count--;
			printf(" $10 ");
		}
		else if(change >= 500 && coinregister[FIVE_DOLLARS].count > 0)
		{
			change -= 500;
			coinregister[FIVE_DOLLARS].count--;
			printf(" $5 ");
		}
		else if(change >= 200 && coinregister[TWO_DOLLARS].count > 0)
		{
			change -= 200;
			coinregister[TWO_DOLLARS].count--;
			printf(" $2 ");
		}
		else if(change >= 100 && coinregister[ONE_DOLLAR].count > 0)
		{
			change -= 100;
			coinregister[ONE_DOLLAR].count--;
			printf(" $1 ");
		}
		else if(change >= 50 && coinregister[FIFTY_CENTS].count > 0)
		{
			change -= 50;
			coinregister[FIFTY_CENTS].count--;
			printf(" 50c ");
		}
		else if(change >= 20 && coinregister[TWENTY_CENTS].count > 0)
		{
			change -= 20;
			coinregister[TWENTY_CENTS].count--;
			printf(" 20c ");
		}
		else if(change >= 10 && coinregister[TEN_CENTS].count > 0)
		{
			change -= 10;
			coinregister[TEN_CENTS].count--;
			printf(" 10c ");
		}
		else if(change >= 5 && coinregister[FIVE_CENTS].count > 0)
		{
			change -= 5;
			coinregister[FIVE_CENTS].count--;
			printf(" 5c ");
		}
		else
		{
			printf("Unable to complete sale, insufficient change available\n");
			return;
		}
	}

}

char *printDenom(VmSystem *system, int index)
{
	Denomination denom = system->cashRegister[index].denom;

		switch(denom) 
	{
	    case TEN_DOLLARS:
            return "10 dollar";
    	case FIVE_DOLLARS:
            return "5 dollar";
        case TWO_DOLLARS:
            return "2 dollar";
        case ONE_DOLLAR:
            return "1 dollar";
        case FIFTY_CENTS:
            return "50 cents";
        case TWENTY_CENTS:
            return "20 cents";
        case TEN_CENTS:
            return "10 cents";
        case FIVE_CENTS:
            return "5 cents";
    }

    return NULL;
}