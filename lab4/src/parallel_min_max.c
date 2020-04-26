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
void signal_alarms();
int main(int argc, char** argv) {
    int seed = -1;
    int array_size = -1;
    int pnum = -1;
    int timeout = -1;
    bool with_files = false;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = { {"seed", required_argument, 0, 0},
                                          {"array_size", required_argument, 0, 0},
                                          {"pnum", required_argument, 0, 0},
                                          {"by_files", no_argument, 0, 'f'},
                                          {"timeout",required_argument,0,0},
                                          {0, 0, 0, 0} };
        int option_index = 0;
        int c = getopt_long(argc, argv, "f", options, &option_index);

        if (c == -1) break;

        switch (c) {
        case 0:
            switch (option_index) {
            case 0:
                seed = atoi(optarg);
                if (seed <= 0)
                {
                    printf("seed < 0 попробуйте снова\n\n");
                    return 1;
                }
                break;
            case 1:
                array_size = atoi(optarg);
                if (array_size <= 0)
                {
                    printf("Недопустимый array_size Попробуйте снова\n");
                    return 1;
                }
                break;
            case 2:
                pnum = atoi(optarg);
                if (array_size <= 0)
                {
                    printf("pnum меньше нуля! Попробуйте снова!\n");
                    return 1;
                }
                if (pnum > array_size)
                {
                    printf("Неккоректное значение pnum! Попробуйте снова!\n");
                    return 1;
                }
                break;
           
            case 3:
                with_files = true;
                break;
             case 4:
                timeout = atoi(optarg);
                if (timeout <= 0)
                {
                    printf("Недопустимый timeout Попробуйте снова\n");
                    return 1;
                }
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

    int* array = malloc(sizeof(int) * array_size);
    GenerateArray(array, array_size, seed);
    int active_child_processes = 0;
int i;
int* fork_nums =  malloc(sizeof(int) * pnum);
for(i=0;i<pnum;i++)
{
    fork_nums[i] = INT_MIN;
}
    struct timeval start_time;
    struct singal_alarms
    {
        int * forks;
        int pnums;
    } sigs;
    gettimeofday(&start_time, NULL);
    int pipefd[2];
    pipe(pipefd);
    int size = array_size / pnum;
                    if(timeout != -1)
                {
                    printf("Установка таймера для принудительной остановки ДП\n");
                    signal (SIGALRM, signal_alarms);//Привязка сигнала SIGALRM к функции signal_alarms
                    alarm(timeout); //Установка таймера на заданное время после истечения времени посылается SIGALRM и выполняется функция signal_alarms
                }
    for ( i = 0; i < pnum; i++) {
        pid_t child_pid = fork();
        if (child_pid >= 0) {
            active_child_processes += 1;
            if (child_pid == 0) {
                setpgrp();


                struct MinMax DataMinMax;
                if (i != pnum - 1) 
                {
                    DataMinMax = GetMinMax(array, i * size, (i + 1) * size);
                }
                else  DataMinMax = GetMinMax(array, i * size, array_size);
                if (with_files) {
                    FILE* file = fopen("data.txt", "a");
                    fwrite(&DataMinMax, sizeof(struct MinMax), 1, file);
                    fclose(file);
                }
                else {
                    write(pipefd[1], &DataMinMax, sizeof(struct MinMax));
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
        close(pipefd[1]);
        wait(NULL);
        active_child_processes -= 1;
    }
    struct MinMax min_max;
    min_max.min = INT_MAX;
    min_max.max = INT_MIN;
    for ( i = 0; i < pnum; i++) {
        int min = INT_MAX;
        int max = INT_MIN;

        struct MinMax DataMinMax;

        if (with_files) {
            FILE* file = fopen("data.txt", "r");
            fseek(file, i * sizeof(struct MinMax), SEEK_SET);
            fread(&DataMinMax, sizeof(struct MinMax), 1, file);
            fclose(file);
        }
        else {
            read(pipefd[0], &DataMinMax, sizeof(struct MinMax));
        }
        min = DataMinMax.min; max = DataMinMax.max;

        if (min < min_max.min) min_max.min = min;
        if (max > min_max.max) min_max.max = max;
    }

    struct timeval finish_time;
    gettimeofday(&finish_time, NULL);
    double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
    elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
    free(array);
    remove("data.txt");
    printf("Min: %d\n", min_max.min);
    printf("Max: %d\n", min_max.max);
    printf("Elapsed time: %fms\n", elapsed_time);
    fflush(NULL);
    return 0;
}
void signal_alarms()
{
    printf("Принудительная остановка дочерних процессов!\n");
    kill(0,SIGKILL);
}