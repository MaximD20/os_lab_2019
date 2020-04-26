
CC=gcc
CFLAGS=-I.

all: parallel_min_max parallel_sum process_memory
#Для задания 1
parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
#Для задания 3
process_memory : 
	$(CC) -o process_memory process_memory.c $(CFLAGS)
#Для задания 5
parallel_sum : utils.o utils.h 
	$(CC) -o parallel_sum utils.o parallel_sum.c $(CFLAGS) -lpthread

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_sum parallel_min_max process_memory
