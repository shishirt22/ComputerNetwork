#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
int no_client=2;
pthread_mutex_t mutex;
void * WORK(void * arg)
{
	int fd=*((int *)arg);
	sleep(10);
	write(fd,"Hello",6);
	pthread_mutex_lock(&mutex);
	no_client++;
	close(fd);
	pthread_mutex_unlock(&mutex);
}
int main()
{
	struct sockaddr_in client;
	pthread_mutex_init(&mutex,NULL);
	int flag=1,clilen;
	fd_set rfds;
	int ret;
	struct timeval T;
	while(1)
	{
		T.tv_sec=0;
		T.tv_usec=700;
		if(flag)
		{
			FD_ZERO(&rfds);
			FD_SET(0,&rfds);
		}
		ret=select(1,&rfds,NULL,NULL,&T);
		if(ret>0){
			printf("INCOMING CONNECTION\n");
			pthread_mutex_lock(&mutex);
			if(no_client<=0)
			{
				printf("WAIT");
				flag=0;
			}
			else
			{
				flag=1;
				clilen=sizeof(client);
				int temp;
				temp=accept(0,(struct sockaddr *)&client,&clilen);
				no_client--;
				pthread_t worker;
				pthread_create(&worker,NULL,WORK,&temp);
			}
			pthread_mutex_unlock(&mutex);
		}
	}
}