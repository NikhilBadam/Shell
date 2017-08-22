#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>
void otherargument(char * othercommand[]){
	pid_t pid;
	pid=fork();
	if(pid<0){
		perror("Error in creating child process\n");
	}
	if(pid==0){
	//	printf(" ");
		int check=execvp(*othercommand,othercommand);
		if(check<0){
			perror("enter correct command\n");
		}
	}
	else{
		wait(NULL);
	}
}
