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
void echoargument(char * echocommand[],int i){
	pid_t pid;
	pid=fork();
	if (pid<0){
		perror("Error in creating child process\n");
	}
	if(pid==0){
		int k;
		for(k=1;k<i;k++){
			char test[1000];
			strcpy(test,echocommand[k]);
			int cut=strlen(test);
			int l;
			if(test[0]=='"'){
				for(l=0;l<cut-2;l++){
					test[l]=test[l+1];
				}
				test[l]='\0';
				strcpy(echocommand[k],test);
			}
			else if(test[0]=='$'){
				for(l=0;l<cut-1;l++){
					test[l]=test[l+1];
				}
				test[l]='\0';
				const char* temp=getenv(test);
				strcpy(echocommand[k],temp);
			}
		}
		int check=execvp(*echocommand,echocommand);
		if(check<0){
			perror("Enter Correct Command\n");
		}
	}
	else{
		wait(NULL);
	}
}
