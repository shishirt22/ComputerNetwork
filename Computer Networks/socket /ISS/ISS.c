#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
struct Protocol{
	char path[20];
	int portno;
};
fd_set rfds;
struct timeval T;
struct Protocol P[4];
void sig()
{
	perror("SIGNAL RECEIVED\n");
}
int main(int argn,char * argc[])
{
	int i;
	freopen("init.txt","r",stdin);
	perror("");
	for(i=0;i<4;i++)
	{
		scanf("%s %d",&(P[i].path),&(P[i].portno));
		printf("%s %d",P[i].path,P[i].portno);
	}
	struct  sockaddr_in server[4],client;
	int portno,clilen,sfd[4],cfd,mfds,ret;
	signal(SIGCHLD,sig);
	//memset(&client,0,sizeof(client));
	//portno=atoi(argc[1]);
	for(i=0;i<4;i++){
		sfd[i]=socket(AF_INET,SOCK_STREAM,0);
		bzero(&server[i],sizeof(server[i]));
		server[i].sin_family=AF_INET;
		server[i].sin_addr.s_addr=INADDR_ANY;
		server[i].sin_port=htons(P[i].portno);
		if(bind(sfd[i],(struct sockaddr *)&server[i],sizeof(server[i])))
		{
			perror("BIND FAILED");
			return 0;
		}
		else
		{
			printf("Successs");
		}
		listen(sfd[i],10);
	}
	int pid,j;
	clilen=sizeof(client);
	printf("\t\tSERVER RUNNING\n");
	while(1)
	{
		FD_ZERO(&rfds);
		mfds=-10;
		for(i=0;i<4;i++)
		{
			FD_SET(sfd[i],&rfds);
			T.tv_sec=0;
			T.tv_usec=5;
			mfds=mfds<sfd[i]?sfd[i]:mfds;
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			perror("Connection Coming");
			for(i=0;i<4;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{
					bzero(&client,sizeof(client));
					clilen=sizeof(client);
					cfd=accept(sfd[i],(struct sockaddr *)(&client),&clilen);
					pid=fork();
					if(pid==0)
					{
						for(j=0;j<4;j++)
							close(sfd[j]);
						dup2(cfd,0);
						dup2(cfd,1);
						execv(P[i].path,NULL);
						perror("EXEC ERROR");
					}
					else if(pid>0)
					{
						close(cfd);
					}
					else
					{
						perror("FORK ERROR");
					}
				}
			}
		}
	}
}