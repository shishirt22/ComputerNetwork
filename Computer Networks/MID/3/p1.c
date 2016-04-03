#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "markup.h"

void handler()
{
	printf("HAndling ");
	fflush(stdout);
	alarm(3);
}
char buf[1000];
int main()
{
	int p6fd[2];
	pipe(p6fd);
	int pid=fork();
	if(pid==0)
	{
		close(p6fd[1]);
		dup2(p6fd[0],0);
		execv("p6",NULL);
	}
	close(p6fd[0]);
	int p2fd[2];
	pipe(p2fd);
	pid=fork();
	if(pid==0)
	{
		close(p6fd[1]);
		close(p2fd[0]);
		dup2(p2fd[1],1);
		execv("p2",NULL);
	}
	close(p2fd[1]);
	int p3fd[2];
	pipe(p3fd);
	pid=fork();
	if(pid==0)
	{
		close(p6fd[1]);
		close(p2fd[0]);
		close(p3fd[0]);
		dup2(p3fd[1],1);
		execv("p3",NULL);
	}
	close(p3fd[1]);
	int p4fd[2];
	pipe(p4fd);
	pid=fork();
	if(pid==0)
	{
		close(p6fd[1]);
		close(p3fd[0]);
		close(p2fd[0]);
		close(p4fd[0]);
		dup2(p4fd[1],1);
		execv("p4",NULL);

	}
	close(p4fd[1]);
	int p5fd[2];
	pipe(p5fd);
	pid=fork();
	if(pid==0)
	{
		close(p6fd[1]);
		close(p2fd[0]);
		close(p3fd[0]);
		close(p4fd[0]);
		close(p5fd[0]);
		dup2(p5fd[1],1);
		execv("p5",NULL);
	}
	int fd[2];
	pipe(fd);
	dup2(fd[1],1);
	fg(p2fd[0],p3fd[0]);
	fg(p4fd[0],p5fd[0]);
	fg(fd[0],fd[0]);
	struct pollfd Fd[4];
	Fd[0].fd=p2fd[0];
	Fd[1].fd=p3fd[0];
	Fd[2].fd=p4fd[0];
	Fd[3].fd=p5fd[0];
	Fd[0].events=POLLIN;
	Fd[1].events=POLLIN;
	Fd[2].events=POLLIN;
	Fd[3].events=POLLIN;
	int nsfd=4,ret,a,i;
	signal(SIGALRM,handler);
	printf("AVAILABLE ");
	fflush(stdout);
	alarm(2);
	//close(fd[1]);
	while(1)
	{
		ret=poll(Fd,nsfd,0);
	//	printf("%d",ret);
	//	perror("POLLIN");
		if(ret>0)
		{
		//	perror("LOO");
			for(i=0;i<4;i++)
			{
				if(Fd[i].revents & POLLIN)
				{
					while((a=read(Fd[i].fd,buf,sizeof(buf)))>0)
					{
						write(p6fd[1],buf,a);
					}
				}
			}
		}
		else
		{
			a=read(fd[0],buf,sizeof(buf));
			if(a>0)
			{
				write(p6fd[1],buf,a);
			}
		}
	}
}