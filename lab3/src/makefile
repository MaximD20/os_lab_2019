#Переменные
#CC обозначает компилятор
#CFLAGS включение заголовочных файлов
CC=gcc
CFLAGS=-I.
#Стандартная цель позволяет собрать все программы за одну команду
all: sequential_min_max parallel_min_max parallel
# Сборка программы sequential_min_max с использованием зависимостей. Зависимости позволяют при изменении одного из файлов не пересобирать весь проект заново
sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)
#Сборка(Линковка) программы parallel_min_max с использованием зависимостей
parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
#Компиляция utils.c
utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)
#Компиляция find_min_max.c
find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)
#Сборка программы для задания 5
parallel: paralleling.c sequential_min_max
	$(CC) -o parallel paralleling.c
#Стандартная цель позволяет с помощью одной команды удалить объектные и исполняемые файлы
clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max parallel
