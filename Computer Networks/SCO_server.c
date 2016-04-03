#include <stdio.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "semaphore.h"
struct SHM{
	int sid,co_id,serv_id,cl_id;
	char wfifo[50],rfifo[50];
	int availablity;
};
struct SHM * S;
int flag,flag3;
void starter()
{
	printf("Server starting\n");
	flag=0;
}
void changeToUpper(char buf[],int len)
{
	int i=0,count=0;
	for(i=0;i<len;i++)
	{
		if(isalpha(buf[i]))
		{
			if(isupper(buf[i]))
				buf[i]=(char)tolower(buf[i]);
			else
				buf[i]=(char )toupper(buf[i]);
		}
		else
			count++;
	}
	if(count>=3)
		{
			printf("Cmplicated\n");
			flag3=1;
		}
}
char buf[1000];
int main(int argn,char * argc[])
{
	int a,ret,flag2,fd1,fd2,ppid,i;
	int id=atoi(argc[1]);
	printf("Id is %d\n",id);
	int key=ftok(".",'e');
	char wfifo[100],rfifo[100];
	int shmid=shmget(key,20*sizeof(struct SHM),0666);
	perror("shm");
	S=(struct SHM *)shmat(shmid,NULL,0);
	key=ftok(".",'a');
	int semid=semget(key,1,0666);
	perror("sem ");
	change(semid,0,-1);
	S[id].sid=id;
	S[id].serv_id=getpid();
	S[id].availablity=1;
	ppid=S[id].co_id;
	strcpy(wfifo,S[id].wfifo);
	strcpy(rfifo,S[id].rfifo);
	change(semid,0,1);
	printf("%s %s\n",S[id].wfifo,S[id].rfifo);
	signal(SIGUSR2,starter);
	while(1)
	{
		while(flag)
		{

		}
		fd1=open(rfifo,O_RDONLY);
		fd2=open(wfifo,O_WRONLY);
		//Fd[0].fd=fd;
		//Fd[0].events=POLLIN;
		printf("Going to start\n");
		fflush(stdout);
		flag2=1;
		flag3=0;
		while(flag2)
		{
			a=read(fd1,buf,sizeof(buf)-1);
			if(a>0)
			{
				buf[a]='\0';
				printf("Received %s",buf);
				changeToUpper(buf,a);
				if(flag3)
				{
					flag2=0;
				}
				write(fd2,buf,a);
				///fsync(fd2);
			}
			if(flag2==0)
			{
				printf("ended\n");
				flag=1;
				//write(fd2,"END\0",4);
				kill(ppid,SIGUSR1);
				close(fd1);
				close(fd2);
			}
		}
		flag=1;
	}
}