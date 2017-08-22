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
#include <fcntl.h>
#include "jobs.h"

void direct(char *argument[],int start){
	int f,inflag,outflag,outappflag,gan;
	char infile[100],outfile[100],outappfile[100];
	inflag=0;
	outflag=0;
	outappflag=0;
	gan=0;
	f=start;
	while(argument[f]!=NULL){
		if(strcmp(argument[f],"<")==0){
			strcpy(infile,argument[f+1]);
			argument[f]=NULL;
			argument[f+1]=NULL;
			inflag=1;
			f++;
		}
		else if(strcmp(argument[f],">")==0){
			strcpy(outfile,argument[f+1]);
			argument[f]=NULL;
			argument[f+1]=NULL;
			outflag=1;
			f++;
		}
		else if(strcmp(argument[f],">>")==0){
			strcpy(outappfile,argument[f+1]);
			argument[f]=NULL;
			argument[f+1]=NULL;
			outappflag=1;
			f++;
		}
		else{
			argument[gan]=argument[f];
			gan++;
		}
		f++;
	}
	if(outflag==1){
		int fd;
		fd=creat(outfile,0644);
		if(fd<0){
			perror("Error while opening output file\n");
			exit(0);
		}
		dup2(fd,1);
		close(fd);
	}
	else if(outappflag==1){
		int fd=open(outappfile,O_WRONLY | O_APPEND | O_CREAT);
		if(fd<0){
			perror("Output file not found\n");
			exit(0);
		}
		chmod(outappfile,0777);
		dup2(fd,1);
		close(fd);
	}
	if(inflag==1){
		int fd;
		fd= open(infile, O_RDONLY,0);
		if(fd<0){
			perror("Error while opening input file\n");
			exit(0);
		}
		dup2(fd,0);
		close(fd);
	}
	if(strcmp(argument[start],"echo")==0){
		int k=1;
		while(argument[k]!=NULL){
			char test[1000];
			strcpy(test,argument[k]);
			int cut=strlen(test);
			int l;
			if(test[0]=='"'){
				for(l=0;l<cut-2;l++){
					test[l]=test[l+1];
				}
				test[l]='\0';
				strcpy(argument[k],test);
			}
			else if(test[0]=='$'){
				for(l=0;l<cut-1;l++){
					test[l]=test[l+1];
				}
				test[l]='\0';
				const char* temp=getenv(test);
				strcpy(argument[k],temp);
			}
			k++;
		}
	}
	if(strcmp(argument[start],"jobs")==0){
		jobs();
	}
	else{
		int check=execvp(argument[start],&argument[start]);
		if(check<0){
			perror("Enter Correct Command\n");
		}
	}
}
