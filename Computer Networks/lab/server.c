#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include "socket.h"
#include <pthread.h>
#define sname "127.0.0.1"
void * READ(void * arg)
{
	int fd=*((int *)arg);
	int a,n;
	char buf[100];
	while(1)
	{
		while((a=read(fd,buf,sizeof(buf)))>0)
		{
			buf[a]='\0';
			strcat(buf,"RECV");
			write(fd,buf,strlen(buf));
		}
		//printf("DONE");
	}
}
int main( int arg,char * argc[])
{
	int sfd=mkserver(atoi(argc[1]),5,NULL);
	struct sockaddr_in client;
	int a,nfd[10],clilen;
	pthread_t thread[10];
	int n=0;
	while(1)
	{
		clilen=sizeof(client);
		nfd[n]=accept(sfd,(struct sockaddr *)&client,&clilen);
		perror("");
		nonblock(nfd[n]);
		pthread_create(&thread[n],NULL,READ,(void *)&nfd[n]);
		n++;
	}

}