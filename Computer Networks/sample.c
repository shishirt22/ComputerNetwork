#include <stdio.h>
#include <unistd.h>
char ch1[10];
int sample()
{
	int p1[2];
	pipe(p1);
	int pid=fork();
	if(pid==0)
	{
		close(p1[1]);
		//write(p1[1],"mess",4);
		dup2(p1[0],0);
		//read()
		fflush(0);
		scanf("%s",&ch1);
		printf("%s",ch1);
	//	close(p1[1]);
		exit(0);
	}
	else
	{
		close(p1[0]);
		//read(p1[0],ch1,4);
		write(p1[1],"mess",4);
		//printf("%s\n heelo",ch1);
		//close(p1[0]);
		wait(NULL);
	}
	return 0;
}
