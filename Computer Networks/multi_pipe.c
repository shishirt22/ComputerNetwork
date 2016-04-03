#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define N 1000
char ch[N+1];
int p1[2],p2[2];
int main(int argc,char  ** argv)
{
	int i,j,pid,a;
	//printf("%s",argv[2]);
	pipe(p1);
	//pipe(p2);
//	perror("");
//	fflush(stderr);
	int a2,a3;
	int n=atoi(argv[1]);
	pid=fork();
	if(pid==0)
	{
		close(p1[0]);
		dup2(p1[1],1);
		execv(argv[2],NULL);
		perror("");
		printf("Helloe\n");
		fflush(stdout);
		exit(0);
	}
	else
	{
		a2=dup(p1[1]);
		close(p1[1]);
		wait(NULL);
		a=read(p1[0],ch,N);
		//printf("%s Raeded",ch);
	}
	pipe(p2);
	for(i=1;i<n-1;i++){
		printf("forked");
		fflush(stdout);
		pid=fork();
		if(pid==0)
		{
			printf("p2");
			fflush(stdout);
			//p1[1]=a2;
			close(p1[0]);
			dup2(a2,1);
			close(p2[1]);
			dup2(p2[0],0);
			execv(argv[i+2],NULL);
				perror("");
		printf("Helloe\n");
	fflush(stdout);
		exit(0);

		}
		else if(pid>0)
		{
			if(i==1)
				{
					a3=dup(p2[0]);
					close(p2[0]);
				}
			//a=dup(p2[1]);
			write(p2[1],ch,a);
			//close(p2[1]);
			//p2[1]=a;
			wait(NULL);
			a=read(p1[0],ch,N);
		}
		else
			printf("fork error");
	}
	printf("%s",ch);
	fflush(stdout);
	pid=fork();
	if(pid==0)
	{
		close(p1[0]);
	//	dup2(1,p1[1]);
		p2[0]=a3;
		close(p2[1]);
		dup2(p2[0],0);
		execv(argv[i+2],NULL);
	}
	else
	{
		close(p2[0]);
		write(p2[1],ch,a);
		wait(NULL);
		printf("LAst wriiten by end process\n");
	}
}
