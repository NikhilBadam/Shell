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
#include "direct.h"
#include "jobs.h"
sigjmp_buf env;

void pipe1(char *argument[],int check){
	int i1=0;
	int arr1[100];
	int j1=1;
	arr1[0]=0;
	while(argument[i1]!=NULL){
		if(strcmp(argument[i1],"|")==0){
			arr1[j1]=i1+1;
			j1++;
			argument[i1]=NULL;
		}
		i1++;
	}
	int k1=0,l1=0;
	int fd1[2*(j1-1)];
	for(i1=0;i1<j1-1;i1=i1+1){
		pipe(fd1+2*i1);
	}
	pid_t pid;
	pid=fork();
	i1=1;
	k1=0;
	if(pid==0){
		/*	int ch=0,ck=0,inflag=0;
			char infile[100];
			while(argument[ch]!=NULL){
			if(strcmp(argument[ch],"<")==0){
			strcpy(infile,argument[ch+1]);
			argument[ch]=NULL;
			argument[ch+1]=NULL;
			inflag=1;
			ch=ch+1;
			}
			else{
			argument[ck]=argument[ch];
			ck=ck+1;
			}
			ch=ch+1;
			}
			if(inflag==1){
			int fd;
			fd = open(infile, O_RDONLY,0);
			if(fd<0){
			perror("Error in input file with pipe\n");
			}
			dup2(fd,0);
			close(fd);
			}*/
		dup2(fd1[1],1);
		for(l1=0;l1<2*(j1-1);l1++){
			if(l1!=1){
				close(fd1[l1]);
			}
		}
		direct(argument,0);
		//execvp(*argument,argument);
	}
	else if(pid<0){
		perror("Error pipe first fork\n");
	}
	else{
		wait(NULL);
		//for(i1=1;i1<=(j1-2);i1++){
		while(i1<=j1-2){
			if(fork()==0){
				//write(2,"hai\n",sizeof(int));
				dup2(fd1[2*i1-2],0);
				dup2(fd1[2*i1+1],1);
				for(l1=0;l1<2*(j1-1);l1++){
					if(l1!=(2*i1-2)){
						close(fd1[l1]);
					}
				}
				direct(argument,arr1[i1]);
				//execvp(argument[arr1[i1]],&argument[arr1[i1]]);
			}
			i1=i1+1;
		}
		if(check==0){
			pid=fork();
			if(j1!=1){
				if(pid==0){
					dup2(fd1[2*i1-2],0);
					for(l1=0;l1<2*(j1-1);l1++){
						if(l1!=2*i1-2){
							close(fd1[l1]);
						}
					}
					direct(argument,arr1[i1]);
					//execvp(argument[arr1[i1]],&argument[arr1[i1]]);
				}
			}
		}
		else{
			if(j1!=1){
				int j=arr1[i1];
				while(argument[j]!=NULL){
					j++;
				}
				argument[j-1]=NULL;
				pid_t pid;
				pid=fork();
				if(pid<0){
					perror("Error in creating child process\n");
				}
				if(pid==0){
					setpgid(0,0);
					int check=execvp(argument[arr1[i1]],&argument[arr1[i1]]);
					if(check<0){
						perror("Enter Correct Command\n");
					}
					exit(0);
				}
				else{
					//      wait(NULL);
				}
				insertpid(argument[arr1[i1]],pid);
				/*printf("[1] %d\n",pid);
				struct sigaction sa; 
				sigfillset(&sa.sa_mask);
				sa.sa_handler=(void *)deletebackground;
				sa.sa_flags=0;
				sigaction(SIGCHLD,&sa,NULL);
				sigsetjmp(env,1);*/
			}

		}
	}
	for(l1=0;l1<2*(j1-1);l1++){
		close(fd1[l1]);
	}
	if(check==0){
		for(l1=0;l1<=j1;l1++){
			wait(NULL);
		}
	}
}
