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
struct node{
	char processname[100];
	int processid;
	struct node *next;
};
struct node *root;
void insertpid(char name[],int id){
	if(root==NULL){
		struct node *temp=(struct node *)malloc(sizeof(struct node));
		strcpy(temp->processname,name);
		temp->processid=id;
		temp->next=NULL;
		root=temp;
	}
	else
	{
		struct node *temp=(struct node *)malloc(sizeof(struct node));
		strcpy(temp->processname,name);
		temp->processid=id;
		temp->next=NULL;
		struct node *temp2=root;
		while(temp2->next!=NULL)
			temp2=temp2->next;
		temp2->next=temp;
	}
}
void deletepid(int id){
	if(root!=NULL)
	{
		struct node *temp=root;
		if(root->processid==id)
		{
			root=root->next;
		}
		else
		{
			struct node *temp2;
			while(temp!=NULL&&temp->processid!=id)
			{
				temp=temp->next;
				temp2=temp;
			}
			if(temp!=NULL)
			{
				temp2->next=temp->next;
			}
		}
		if(temp!=NULL){
			free(temp);
		}
	}
	return;
}
int getpi(int jo){
	struct node *temp=root;
	int i=1;
	int pid=0;
	while(temp!=NULL){
		if(i==jo){
			pid=temp->processid;
			break;
		}
		temp=temp->next;
		i=i+1;
	}
	return pid;
}
void jobs(){
	struct node *temp=root;
	int i=1;
	while(temp!=NULL){
		printf("[%d] %s [%d]\n",i,temp->processname,temp->processid);
		i=i+1;
		temp=temp->next;
	}
}
void kjob(char *argument[]){
	int digit,len,jo,i,signal;
	digit=0;
	len=strlen(argument[1]);;
	jo=0;
	i=0;
	while(i<len){
		digit=(int)argument[1][i];
		digit=digit-48;
		jo=10*jo+digit;
		i++;
	}
	int pid=getpi(jo);
	digit=0;
	len=strlen(argument[2]);
	signal=0;
	i=0;
	while(i<len){
		digit=(int)argument[2][i];
		digit=digit-48;
		signal=10*signal+digit;
		i++;
	}
	if(pid==0){
		printf("Enter correct job number\n");
	}
	else{
		int check=killpg(pid,signal);
		if(check<0){
			perror("Enter correct values\n");
		}
	}
}
void fg(char *argument[]){
	int digit,len,job,i;
	digit=0;
	len=strlen(argument[1]);
	job=0;
	i=0;
	while(i<len){
		digit=(int)argument[1][i];
		digit=digit-48;
		job=10*job+digit;
		i++;
	}
	int status;
	int pid=getpi(job);
	if(pid==0){
		printf("No such Background number exists\n");
	}
	else{
		waitpid(pid,&status,0);
	}
}
void killall(){
	while(root!=NULL){
		kill(root->processid, SIGKILL);
		deletepid(root->processid);
	}
}
