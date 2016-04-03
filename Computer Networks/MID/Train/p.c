#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>
#include <poll.h>
char buf[1000];
int main()
{
	struct sockaddr_in client;
	struct pollfd Fd[2];
	int clilen;
	Fd[0].fd=0;
	Fd[0].events=POLLRDNORM|POLLIN;
	int nfds=1,ret,cfd,a;
	while(1)
	{
		ret=poll(Fd,nfds,0);
		if(ret>0)
		{
			if(Fd[0].revents & POLLRDNORM)
			{
				clilen=sizeof(client);
				cfd=accept(Fd[0].fd,(struct sockaddr *)&client,&clilen);
				Fd[nfds].fd=cfd;
				Fd[nfds].events=POLLIN;
				nfds++;
			}
			else if(nfds>1&&Fd[1].revents & POLLIN)
			{
				//printf("TRAIN INFO\n");
				while((a=read(Fd[1].fd,buf,sizeof(buf)))>0)
				{
					buf[a]='\0';
					printf("%s",buf,a);
					fflush(stdout);
				}
				//printf("%d \n",a);
				if(a==0)
				{
					printf("TRAIN LEFT\n");
					nfds--;
					fflush(stdout);
					kill(getppid(),SIGUSR1);
				}
			}
		}
	}
}