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
void pwdargument(){
	char pwd[200];
	getcwd(pwd,sizeof(pwd));
	printf("%s",pwd);
	printf("\n");
}
