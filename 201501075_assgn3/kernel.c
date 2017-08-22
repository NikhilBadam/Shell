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
#include "cd.h"
#include "echo.h"
#include "pinfo.h"
#include "pwd.h"
#include "other.h"
#include "pipe.h"
#include "redirect.h"
#include "jobs.h"

char *user;
char syste[200];
char commandline[1000],pwddup[1000],pwd[1000];
int nik;
sigjmp_buf env;
void deletebackground(){
	pid_t pid;
	int status;
	while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
	{
		printf("\nChild %d terminated\n", pid);
		deletepid(pid);
	}
	siglongjmp(env,1);
}
void promtline(){
	strcpy(commandline,"<");
	strcat(commandline,user);
	strcat(commandline,"@");
	strcat(commandline,syste);
	strcat(commandline,":~");
	getcwd(pwddup,sizeof(pwddup));
	int nikdup=strlen(pwddup);
	if(nikdup>=nik){
		if(strncmp(pwd,pwddup,nik)==0){
			strcat(commandline,pwddup+(nik));
		}
		else{
			strcat(commandline,pwddup);
		}
	}
	else{
		strcat(commandline,pwddup);
	}
	strcat(commandline,"$ ");
	strcat(commandline,"> ");
	printf("%s",commandline);
}
void crlt(int sil){
	printf("\n");
	promtline();
	fflush(stdout);
}
int main(){
	int i,num;
	char *getwd(char *buf);
	char input[500],*argument[10],*token,*token1;
	int col;
	char column[2]=";";
	char *first[100];
	user=getenv("USER");
	const char space[2]=" ",bar[2]="/";
	gethostname(syste,200);
	int j=1;
	getcwd(pwd,sizeof(pwd));
	nik=strlen(pwd);
	void deletebackground();
	while(1){
	/*	strcpy(commandline,"<");
		strcat(commandline,user);
		strcat(commandline,"@");
		strcat(commandline,system);
		strcat(commandline,":~");
		getcwd(pwddup,sizeof(pwddup));
		int nikdup=strlen(pwddup);
		if(nikdup>=nik){
			if(strncmp(pwd,pwddup,nik)==0){
				strcat(commandline,pwddup+(nik));
			}
			else{
				strcat(commandline,pwddup);
			}
		}
		else{
			strcat(commandline,pwddup);
		}
		strcat(commandline,"$ ");
		strcat(commandline,"> ");
		printf("%s",commandline);*/
		promtline();
		signal(SIGINT,crlt);
		char *ni=fgets (input, 500,stdin);
		if(ni==NULL){
			printf("\n");
			exit(0);
		}
		int len=strlen(input);
		if(len>2){
			input[len-1]='\0';
		}
		first[0]=strtok(input,column);
		col=0;
		while(first[col]!=NULL){
			col=col+1;
			first[col]=strtok(NULL,column);
		}
		num=0;
		while(num<col){
			argument[0]=strtok(first[num],space);
			j=0;
			int amcheck=0;
			while(argument[j]!=NULL){
				j=j+1;
				argument[j]=strtok(NULL,space);
			}
			if(strcmp(argument[j-1],"&")==0){
				amcheck=1;
			}
			int redicheck=0,f;
			int pipecheck=0;
			int pi;
			for(pi=0;pi<j;pi++){
				if(strcmp(argument[pi],"|")==0){
					pipecheck=1;
				}
			}
			for(f=0;f<j;f++){
				if(strcmp(argument[f],">")==0 || strcmp(argument[f],"<")==0 || strcmp(argument[f],">>")==0){
					redicheck=1;
				}
			}
			if(pipecheck==1){
				pipe1(argument,amcheck);
			}
			else if(redicheck==1){
				redirect(argument);
			}
			else{
				if(strcmp(argument[j-1],"&")==0){
					argument[j-1]=NULL;
					pid_t pid;
					pid=fork();
					if(pid<0){
						perror("Error in creating child process\n");
					}
					if(pid==0){
						setpgid(0,0);
						int check=execvp(argument[0],argument);
						if(check<0){
							perror("Enter Correct Command\n");
						}
						exit(0);
					}
					else{
						//	wait(NULL);
					}
					insertpid(argument[j-2],pid);
					printf("[1] %d\n",pid);
					struct sigaction sa;
					sigfillset(&sa.sa_mask);
					sa.sa_handler=(void *)deletebackground;
					sa.sa_flags=0;
					sigaction(SIGCHLD,&sa,NULL);
					sigsetjmp(env,1);
				}
				else if(strcmp(argument[0],"jobs")==0){
					jobs();
				}
				else if(strcmp(argument[0],"kjob")==0){
					kjob(argument);
				}
				else if(strcmp(argument[0],"fg")==0){
					fg(argument);
				}
				else if(strcmp(argument[0],"killallbg")==0){
					killall();
				}
				else if(strcmp(argument[0],"exit")==0){
					return 0;
				}
				else if(strcmp(argument[0],"pinfo")==0){
					pinfoargument(argument,j,pwddup);
				}
				else if(strcmp(argument[0],"pwd")==0){
					pwdargument();
				}
				else if(strcmp(argument[0],"cd")==0){
					cdargument(argument[1],pwd);
				}
				else{
					if(strcmp(argument[0],"echo")==0){
						echoargument(argument,j);
					}
					else{
						if(strcmp(argument[0],"kill")==0){
							int pi=0;int p=0,s=0;
							int st=strlen(argument[1]);
							while(p<st){
								pi=(int)argument[1][p];
								pi=pi-48;
								s=s*10+pi;
								p++;
							}
							deletepid(s);
						}
						otherargument(argument);
					}
				}
			}
			num++;	
		}
	}
	return 0;
}
