#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/un.h>
#include "sockunix.h"
#include "socket.h"
#define SOCK_PATH "serversock"
int sfd[10],cfd[10],port[10];
int main(int arg,char * argc[])
{
	struct sockaddr_un server,client;
	struct sockaddr_in server2,client2;
	int a,clilen,ret,portno,i,tsfd;
	int sfd1=socket(AF_UNIX,SOCK_STREAM,0);
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	unlink(server.sun_path);
	if(bind(sfd1,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("BIND ERROR");
	}
	listen(sfd1,10);
	int no=atoi(argc[1]);
	portno=atoi(argc[2]);
	for(i=0;i<no;i++)
	{
		port[i]=portno;
		sfd[i]=mkserver(portno,10,NULL);
		portno++;
		clilen=sizeof(client);
		cfd[i]=accept(sfd1,(struct sockaddr *)&client,&clilen);
		perror("");
		printf("ACCEPTED");
		fflush(stdout);
	}
	fd_set rfds;
	int mfds;
	struct timeval T;
	while(1)
	{
		T.tv_sec=0;
		T.tv_usec=5000;
		//mfds=ifd;
		mfds=-10;
		FD_ZERO(&rfds);
		//FD_SET(ifd,&rfds);
		for(i=0;i<no;i++)
		{
			FD_SET(sfd[i],&rfds);
			mfds=mfds>sfd[i]?mfds:sfd[i];
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			printf("Incominmg");
			fflush(stdout);
			for(i=0;i<no;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{
					printf("Incoming connection");
					fflush(stdout);
					clilen=sizeof(client2);
					tsfd=accept(sfd[i],(struct sockaddr *)&client2,&clilen);
					send_fd(cfd[i],tsfd);
					close(tsfd);
				}
			}
		}
	}
}