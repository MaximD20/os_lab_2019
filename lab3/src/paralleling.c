#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	int pid = fork();

	if ( pid == 0 ) {
		execv( "task1", argv );
	}
	wait( NULL );
	printf( "Выполнение Task1 окончено\n" );
	return 0;
}