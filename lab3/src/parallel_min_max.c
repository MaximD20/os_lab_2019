#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if(seed<0)
            {
            printf("Error value seed! Seed is positive. Try again!\n");
            exit(1);
                break;
            }
            break;
          case 1:
            array_size = atoi(optarg);
             if(array_size<0)
            {
            printf("Error! Array_size is positive! Try again\n");
            exit(1);
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if(array_size %pnum !=0)
            {
                printf("Error! Кол-во потоков должно быть кратно кол-ву элементов! Повторите попытку!\n");
                exit(1);
            }
            if(array_size / pnum <3)
            {
                printf("Error! Слишком большое кол-во процессов для такого кол-ва элементов! Повторите попытку!\n");
                exit(1);
            }
            if(pnum>= array_size)
            {
            printf("Error! Pnum > Array_size. Try again\n");
            exit(1);
            }
            else if(pnum < 1)
            {
                printf("Error! Pnum is positive! Try again\n");
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;
    int i,j;
    int count,k = 0;
    int pipefd[2];
    int pipefd1[2];
    char ** files_names;
    int * znach;
    int ** pnum_mass2;
    znach = malloc(sizeof(int)*pnum);
    znach[0] = 0;
    
      pnum_mass2 = malloc(sizeof(int*)*pnum);
      /*  while(*pnum_mass2)
        {
            *pnum_mass2 = malloc(sizeof(int)*2);
        }*/
    files_names = malloc(sizeof(char*)*pnum);
    if(with_files)
    {
    int * pnum_mass;
    pnum_mass = malloc(sizeof(int)*pnum);
    /*Создание массива файлов взаимодействия*/
    for(i=0; i<pnum; i++ )
    {
       char buff = rand()%26+0x61;
        files_names[i] = malloc(sizeof(char)*5);
        strcpy(files_names[i],&buff);
        strncat(files_names[i], ".txt",4);
    }
    }

    
    /*
    Задание интервалов чтения данных
    */
    for(i = 1; i<pnum+1; i++)
    {
    
        znach[i] = znach[i-1] + array_size/pnum;
        if(i>1)
        {
            znach[i] = znach[i-1] + array_size/pnum ;
        }
    }
  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  for ( i = 0; i < pnum; i++) {
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        // parallel somehow
k=0;
        if (with_files) {
                    
                    FILE * file1 = fopen(files_names[i],"w");
                    for(j=znach[i];j<znach[i+1];j++)
                    {
                        if(j!=0 && j == znach[i])
                        {
                            while(k<znach[i])
                            {
                                array++;
                                k++;
                            }
                        }
                        fprintf(file1, "%d\n", *array);
                        array++;
                    }
                    fclose(file1);
                    array = array;
        } else {
            if(i == 0)
            {

            
                        if (pipe(pipefd) < 0) 
                        {
                            printf( "Pipe failed: ");
                            exit(1);
                        }
                        for(j=znach[i];j<znach[i+1];i++)
                        {
                            if(j!=0 && j == znach[i])
                        {
                            while(k<znach[i])
                            {
                                array++;
                                k++;
                            }
                        }
                            printf("Запись в очередь 1\n");
                            write(pipefd[1],&*array,sizeof(int));
                            close(pipefd[1]);
                            array++;
                        }
                    }
            else
            {
                int val;
                 if (pipe(pipefd1) < 0) 
                        {
                            printf( "Pipe failed: ");
                            exit(1);
                        }
                        for(j=znach[i];j<znach[i+1];i++)
                        {
                            if(j!=0 && j == znach[i])
                        {
                            while(k<znach[i])
                            {
                                array++;
                                k++;
                            }
                        }
                            printf("Запись в очередь 2\n");
                            write(pipefd1[1],&*array,sizeof(int));
                            close(pipefd1[1]);
                            array++;
                        }
                        
                    }
        }
        return 0;
      }
    }
     else {
      printf("Fork failed!\n");
      return 1;
    }
  }
  while (active_child_processes > 0) {
    int  val;
    int min2 = INT_MAX;
    int max2 = INT_MIN;
    if(with_files)
    {
        FILE * f = fopen(files_names[active_child_processes-1],"r+");
        if(f == NULL)
        {
            printf("Error open process2 COmpile \n");
        }
        else
        {
            while(!feof(f))
            {
                if(fscanf(f,"%d\n",&val))
                    {
                        
                        if(val<min2)
                        {
                            printf("Для файла %s установлен мин. элемент %i\n", files_names[active_child_processes-1],val);
                        min2= val;
                        }
                        if(val>max2)
                        {
                            printf("Для файла %s установлен макс. элемент %i\n", files_names[active_child_processes-1],val);
                        max2 = val;
                        }
                    }
            }
            fclose(f);
            FILE *f = fopen(files_names[active_child_processes-1],"w+");
            fprintf(f, "%d\n", min2);
            fprintf(f, "%d\n",max2);
            fclose(f);
        }
    }
    else
    {
        int p;
        if(active_child_processes == 2)
        {

        
           while(read(pipefd1[0],&p, 1)>0)
            { 
                if (p < min2)
                min2 = p;
                if(p>max2)
                max2 = p;
            }
            write(pipefd1[1],&min2,sizeof(int));
        write(pipefd1[1],&max2,sizeof(int));
        }
        else
        {
            while(read(pipefd[0],&p, 1)>0)
            { 
                if (p < min2)
                min2 = p;
                if(p>max2)
                max2 = p;
            }
            write(pipefd[1],&min2,sizeof(int));
        write(pipefd[1],&max2,sizeof(int));
        }
    }

    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for ( i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
            int val, count = 0;
      FILE * f1 = fopen(files_names[i],"r");
        if(!f1)
            printf("Error open process1.txt \n");
        else
        {
            while(!feof(f1))
            {
                if(fscanf(f1,"%d\n",&val))
                    {
                    if(count == 0)
                    min = val;
                    else
                    max = val;
                    }
                count++;
            }
            fclose(f1);
        }
    } else {
        if(i==0)
        {
            read(pipefd[0],&min, sizeof(int));
      read(pipefd[0],&max, sizeof(int));
        }
        else
        {
             read(pipefd1[0],&min, sizeof(int));
      read(pipefd1[0],&max, sizeof(int));
        }
        }
    

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
  }