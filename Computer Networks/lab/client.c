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
	int a;
	printf("%d\n",fd);
	printf("RUNNING");
	fflush(stdout);
	char buf[100];
	while(1)
	{
		write(fd,"HELLO",5);
	//	perror("");
		while((a=read(fd,buf,sizeof(buf)))>0)
		{
			buf[a]='\0';
			printf("%s ",buf);
			fflush(stdout);
		}
		sleep(10);
		//printf("DONE");
	}
}
int main(int arg,char * argc[])
{

	pthread_t thread[10];
	char buf[100];
	int sfd[10];
	int n=0,a;
	while(1)
	{
		printf("ENTER THE PORNTO");
		scanf("%d",&a);
		strcpy(buf,"");
		sprintf(buf,"%d",a);
		sfd[n]=mkserver(atoi(argc[1]),0,sname);
		write(sfd[n],buf,strlen(buf));
		printf("%d\n",sfd[n]);
		nonblock(sfd[n]);
		pthread_create(&thread[n],NULL,READ,(void *)&sfd[n]);
		n++;
	}
}