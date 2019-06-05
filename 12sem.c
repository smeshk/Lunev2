#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
int main(int argc, char const *argv[]) {
	int i = 0;
	long int num = 0;
	if(argc == 1) {
		printf("No arguments\n");
		return 0;
	}
	if(argc > 2) {
		printf("Too many\n");
		return 0;
	}
	char *strerr;
	num = strtol(argv[1], &strerr, 0);
	if(*strerr != '\0' || errno == ERANGE) {
		printf("Wrong\n");
		return 0;
	}
	int pid = fork();
	int p;
	for (p = 1; p <= (num - 1) && pid != 0; p++) {
		pid = fork();

	}

	
	return 0;

}