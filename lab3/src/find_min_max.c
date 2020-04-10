#include "find_min_max.h"
#include <stdio.h>
#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  int j,k=0;
 // printf("Просматриваемый промежуток: от %i do %i", begin,end);
for(j=begin;j<end;j++)
{
    if(j!=0 && j==begin)
    {
        for(k =0; k<begin;k++)
        {
            array++;
        }
    }
    printf("Элемент: %i\n",*array);
    if(*array >min_max.max)
    {

        min_max.max = *array;
       // printf("Поменялось значение максимума. Текущее значение: %i\n",*array);
    }
    else if(*array < min_max.min)
    {
        min_max.min = *array;
        //printf("Поменялось значение минимума. Текущее значение: %i\n",*array);
    }
    array++;
}
  // your code here
  return min_max;
}
