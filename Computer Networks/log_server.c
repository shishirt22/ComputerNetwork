#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/types.h>
#include <poll.h>
struct pollfd Fd[2];
char buf[1000];
int main(int argn,char * argc[])
{
	int n,a,ret,i,pid;
	char * myfifo="/tmp/log_server";
	mkfifo(myfifo,0666);
	char name[10];
	int fd=open(myfifo,O_RDONLY|O_NONBLOCK);
	Fd[0].fd=fd;
	Fd[0].events=POLLIN;
	n=atoi(argc[1]);
	for(i=1;i<=n;i++)
	{
		strcpy(name,"S");
		buf[0]=(char)(i+'0');
		buf[1]='\0';
		strcat(name,buf);
		printf("%s",name);
		fflush(stdout);
		pid=fork();
		if(pid==0)
		{
			close(fd);
			fd=open(myfifo,O_WRONLY);
			close(1);
			dup(fd);
			execv(name,NULL);
			perror("");
		}
		else
		{

		}
	}
	while(1)
	{
		ret=poll(Fd,1,0);
		if(ret>0)
		{
			if(Fd[0].revents & POLLIN)
			{
				while((a=read(Fd[0].fd,buf,sizeof(buf)-1))>0)
				{
					buf[a]='\0';
					printf("%s",buf);
					fflush(stdout);
				}
			}
		}
	}
}