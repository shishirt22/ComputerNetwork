#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include "semaphore.h"
struct SHM{
	int sid,co_id,serv_id,cl_id;
	char wfifo[50],rfifo[50];
	int availablity;
};
struct message_info{
	int sid;
	int client_id;
	char wfifo[50];
	char rfifo[50];
};
struct SHM * S;
struct message_info M;
int semid,n;
void serv_handler(int signum,siginfo_t * T,void * arg2)
{
	int i;
	change(semid,0,-1);
	int pid=T->si_pid;
	for(i=1;i<=n;i++)
	{
		if(S[i].serv_id==pid)
		{
			break;
		}
	}
	//kill(S[i].cl_id,SIGUSR1);
	S[i].availablity=1;
	printf("Server with pid %d is going to end connection with client with pid %d\n",S[i].serv_id,S[i].cl_id);
	change(semid,0,1);
}
int main(int argn,char * argc[])
{
	int key=ftok(".",'e');
	char name[100];
	int flag=1,i,a;
	struct sigaction sa;
	int shmid=shmget(key,20*sizeof(struct SHM),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("shm");
	}
	S=(struct SHM *)shmat(shmid,NULL,0);
	perror("adr");
	key=ftok(".",'a');
	semid=semget(key,1,IPC_CREAT|0666);
	if(semid<0)
	{
		perror("sem");
	}
	setval(semid,0,1);
	//printf("Hello %d",argn);
	fflush(stdout);
	change(semid,0,-1);
	//printf("acquired");
	fflush(stdout);
	n=atoi(argc[1]);
	//n=n1;
	printf("%d\n",n);
	fflush(stdout);
	strcpy(name,"/tmp/");;
	char temp[50],temp2[50],temp3[50];
	//np need of  S[i].fifo
	for(i=1;i<=n;i++)
	{
		strcpy(temp,name);
		strcat(temp,itoa(i,temp2,10));
		S[i].sid=i;
		S[i].co_id=getpid();
		strcpy(temp3,temp);
	//	strcpy(S[i].fifo,temp);
		strcat(temp,"w");
		printf("%s ",temp);
		strcpy(S[i].wfifo,temp);
		strcpy(temp,temp3);
		strcat(temp,"r");
		printf("%s ",temp);
		strcpy(S[i].rfifo,temp);
		if(mkfifo(S[i].rfifo,0666)<0)
		{
			perror("fifo server");
		}
		if(mkfifo(S[i].wfifo,0666)<0)
		{
			perror("fifo serevr  reader");
		}
		S[i].availablity=0;
		printf("\n");
	}
	fflush(stdout);
	change(semid,0,1);
	char * myfifo="/tmp/service_cord";
	mkfifo(myfifo,0666);
	perror("mkfifo ");
	int fd1=open(myfifo,O_RDONLY);
	if(fd1<0)
	{
		perror("fifo");
		return 0;
	}
	char  * myfifo2="/tmp/service_cord2";
	if(mkfifo(myfifo2,0666)<0)
	{
		perror(" fifo2 ");
	}
	perror("fifo2 ");
	int fd2=open(myfifo2,O_WRONLY);
	if(fd2<0)
	{
		perror("fifo 2");
		return 0;
	}
	sa.sa_sigaction=&serv_handler;
	sa.sa_flags=SA_SIGINFO;
	sigfillset(&sa.sa_mask);
	sigaction(SIGUSR1,&sa,NULL);
	//sigaction(SIGUSR2,&sa,NULL);
	while(1){

		if((a=read(fd1,&M,sizeof(struct message_info)))>0){
			flag=0;
			printf("%d request received %d\n",a,M.client_id);
			change(semid,0,-1);
			for(i=1;i<=n;i++)
			{
				if(S[i].sid==M.sid)
				{
					flag=1;
					break;
				}
			}
			if(flag)
			{
				printf("found one\n");
				if(S[i].availablity)
				{
					strcpy(M.wfifo,S[i].rfifo);
					strcpy(M.rfifo,S[i].wfifo);
					printf("%s %s\n",M.wfifo,M.rfifo);
					S[i].cl_id=M.client_id;
					kill(S[i].serv_id,SIGUSR2);   //sending signal to process o get ready to server
					S[i].availablity=0;
					M.sid=1;  //suceess to client
					write(fd2,&M,sizeof(struct message_info));
					printf("Assigned server %d with pid %d to Client with  pid %d \n",S[i].sid,S[i].serv_id,S[i].cl_id);
				}
				else
				{
					printf("Server engaged now Try again Later\n");
					M.sid=0;
					write(fd2,&M,sizeof(struct message_info));
				}
			}
			else
			{
				printf("No server found with given server id\n");
				M.sid=-1;
				write(fd2,&M,sizeof(struct message_info));
			}
			change(semid,0,1);
		}
	}
}
/*Improvements <========>
	1.client should immediatedly end connection
	2.change the order of opening od sc_cl and sc_s
	3.test rigrously
*/