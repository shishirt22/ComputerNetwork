#include <stdio.h>
#include <fcntl.h>
char ch[100];
int d;
int main()
{
	int fd=open("/tmp/inetd",O_WRONLY);
	dup2(fd,1);
	while(1)
	{
		scanf("%s %d",&ch,&d);
		if(d==-1)
			break;
		printf("%s %d\n",ch,d);
		fflush(stdout);
	}
}