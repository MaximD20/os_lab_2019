#include <stdio.h>
#include <stdlib.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  int seed = atoi(argv[1]);
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }

  int array_size = atoi(argv[2]);
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

  int *array = malloc(array_size * sizeof(int));
  GenerateArray(array, array_size, seed);
  struct MinMax min_max = GetMinMax(array, 0, array_size);
  free(array);

  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);

  return 0;
}
/*Пояснение по принципу работы:
При компиляции программы кол-во аргументов должно быть обязательно равно 3
Второй аргумент отвечает за начальное значение генерируемого массива чисел, если оно меньше нуля, скрипт выдаст ошибку
Третий аргумент отвечает за размерность массива. Обязательно положительный.
Далее выделяется память под элементы
Генерируется массив
При помощи скрипта find_min_max получаются минимальный и максимальный элементы массива.
Полученные данные печатаются в консоль*/