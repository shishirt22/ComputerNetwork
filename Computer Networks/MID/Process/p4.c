#include <stdio.h>
#include <signal.h>
int main(int arg,char * argc[])
{
	if(arg<2)
	{
		printf("Inavlid argument");
		return 0;
	}
	int pid=atoi(argc[1]);
	char ch[2];
	while(1)
	{
		printf("Enter any key to send signal");
		scanf("%s",&ch);
		printf("SENDING SIGNAL\n");
		kill(pid,SIGUSR1);
	}
}