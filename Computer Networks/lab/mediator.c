#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include "socket.h"
#include <pthread.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#define sname "127.0.0.1"

pthread_mutex_t mutex[5];
int portno[5],id1[5],cfds[5];
int fd[5][100];
void * READ(void * arg)
{
	int id=*((int *)arg);
	struct timeval T;
	int mfds;
	fd_set rfds;
	printf("RUNNING  %d \n",id);
	fflush(stdout);
	char buf[100];
	int a,dest,ret,i;
	while(1)
	{
		pthread_mutex_lock(&mutex[id]);
		if(cfds[id]<=0)
			{
				pthread_mutex_unlock(&mutex[id]);
				continue;
			}
			FD_ZERO(&rfds);
			mfds=-10;
			T.tv_sec=0;
			T.tv_usec=5000;
			for(i=0;i<cfds[id];i++)
			{
				mfds=mfds>fd[id][i]?mfds:fd[id][i];
				FD_SET(fd[id][i],&rfds);
			}
			ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			printf("DOENE");
			for(i=0;i<cfds[id];i++)
			{
				if(FD_ISSET(fd[id][i],&rfds))
				{
					printf("READING\n");
					fflush(stdout);
					if(i%2==0)
						dest=i+1;
					else
						dest=i-1;
					while((a=read(fd[id][i],buf,sizeof(buf)))>0)
					{
						buf[a]='\0';
					//	printf("%s",buf);
						fflush(stdout);
						write(fd[id][dest],buf,strlen(buf));
					}
					printf("WRITING DONE\n");
					fflush(stdout);
				}
			}
		}
		pthread_mutex_unlock(&mutex[id]);
	}
}
int flag[5];
int main(int arg,char * argc[])
{
	int sfd,clilen,cfd,tsfd,a,num,i;
	pthread_t thread[5];
	char buf[100];
	struct sockaddr_in server,client;
	sfd=mkserver(8000,5,NULL);
	for(i=0;i<5;i++)
	{
		pthread_mutex_init(&mutex[i],NULL);
		id1[i]=i;
		cfds[i]=0;
		portno[i]=8001+i;
		flag[i]=0;
	}
	/*for(i=0;i<5;i++)
	{
		pthread_create(&thread[i],NULL,READ,(void *)&id[i]);
	}*/
	while(1)
	{
		clilen=sizeof(client);
		cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
		perror("CLIENT CONNECTED\n");
		a=read(cfd,buf,sizeof(buf));
		buf[a]='\0';
		num=atoi(buf);
		if(flag[num-1]==0)
		{
			pthread_create(&thread[num-1],NULL,READ,(void *)&id1[num-1]);
			flag[num-1]=1;
		}
		pthread_mutex_lock(&mutex[num-1]);
		printf("ACQUIERED");
		fflush(stdout);
		tsfd=mkserver(portno[num-1],0,sname);
		nonblock(tsfd);
		nonblock(cfd);
		fd[num-1][cfds[num-1]]=cfd;
		//fd[num-1][cfds[num-1]].events=POLLIN|POLLRDNORM;
		cfds[num-1]++;
		fd[num-1][cfds[num-1]]=tsfd;
	//	fd[num-1][cfds[num-1]].events=POLLIN|POLLRDNORM;
		cfds[num-1]++;
		pthread_mutex_unlock(&mutex[num-1]);
	}
}
