#include "revert_string.h"

void RevertString( char *str)
{
    int i;
    int n = 0;
    char * ptr = str;
    while(*ptr)
    {
        ptr++;
        n++;
    }
	for ( i = 0; i < n / 2; i++ )
    {
        char c = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = c;
    }

}

