#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	int pid = fork();

	if ( pid == 0 ) {
		execv( "sequential_min_max", argv );
	}
	wait( NULL );
	printf( "Sequential_min_max is finished\n" );
	return 0;
}