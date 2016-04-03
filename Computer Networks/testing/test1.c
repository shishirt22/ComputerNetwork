#include <stdio.h>
#include <signal.h>
int main()
{
	char buf[100];
	int i;
	printf("hello");
	fflush(stdout);
	for(i=0;i<3;i++)
	{
		scanf("%s",&buf);
		printf("Word read from User is ->%s",buf);
		fflush(stdout);
	}
	kill(getppid(),SIGCHLD);
	
}