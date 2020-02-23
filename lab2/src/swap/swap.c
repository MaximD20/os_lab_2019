#include "swap.h"

void Swap(char *left, char *right)
{
	char  r;
    r = left[0];
    left[0] = right[0];
    right[0] = r;

}

