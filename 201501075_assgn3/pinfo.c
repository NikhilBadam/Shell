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
void pinfoargument(char *argument[],int num,char pwd[]){
	char gpid1[1256],gpid2[1256];
	FILE *filepoint;
	int pid;
	long unsigned int s;
	char name[1256],status;
	if(num==1)
	{
		sprintf(gpid1,"/proc/%d/stat",getpid());                                 //open the stat and exe files
		sprintf(gpid2,"/proc/%d/exe",getpid());
	}
	else
	{
		sprintf(gpid1,"/proc/%s/stat",(argument[1]));
		sprintf(gpid2,"/proc/%s/exe",(argument[1]));
	}
	if((filepoint=fopen(gpid1,"r"))!=NULL)
	{
		/*scan the stat file*/
		fscanf(filepoint,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid,&status,&s);  
		fclose(filepoint);
		printf("pid -- %d\nProcess Status -- %c\nmemory -- %lu\n",pid,status,s);
		readlink(gpid2,name,256);
		if(strstr(name,pwd))
		{
			char fi[1000]="~";
			strcat(fi,name+strlen(pwd));
			printf("Executable Path -- %s\n",fi);                           //print print the stuff

		}
		else{
			printf("Executable Path -- %s\n",name);
		}
	}
	else{
		perror("No such process");
	}
}
