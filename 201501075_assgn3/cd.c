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
void cdargument(char * cdcommand,char *pwd){
	if(cdcommand==NULL){
		int check=chdir(pwd);
		if(check<0){
			perror("Error while changing Directory\n");
		}
	}
	else{
		char u=cdcommand[0];
		if(u == '~'){
			char arg[1000];
			strcpy(arg,pwd);
			strcat(arg,cdcommand+1);
			int check=chdir(arg);
			if(check<0){
				perror("Error while changing Directory\n");
			}
		}
		else if(cdcommand[0]=='.'){
			int check=chdir(cdcommand);
			if(check<0){
				perror("Error while changing Directory\n");
			}
		}
		else if(cdcommand[0]=='/'){
			char pwddup[1000];
			getcwd(pwddup,sizeof(pwddup));
			strcat(pwddup,"/");
			strcat(pwddup,cdcommand);
			int check=chdir(pwddup);
			if(check<0){
				perror("Error while changing Directory\n");
			}
		}
		else{
			int check=chdir(cdcommand);
			if(check<0){
				perror("Enter Correct  Directory\n");
			}
		}
	}
}
