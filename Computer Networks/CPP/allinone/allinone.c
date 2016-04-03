#include <stdio.h>
#include <sys/types.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include "../semaphore.h"
#include <sys/shm.h>
struct pollfd Fd[4];
int sfd,rfd;
char buf[1000];
struct SHM{
	int pid;
};
struct SHM * S;
void toggler()
{
	printf("Toggling\n");
	fflush(stdout);
	sfd=sfd==1?rfd:1;
}
int main(int argn,char * argc[])
{
	int ret,a,i;
	int key=ftok(".",'e');
	printf("%d ",getpid());
	int shmid=shmget(key,sizeof(struct SHM),IPC_CREAT|0666);
	S=(struct SHM *)shmat(shmid,NULL,0);
	key=ftok(".",'b');
	int semid=semget(key,1,IPC_CREAT|0666);
	setval(semid,0,0);
	S[0].pid=getpid();
	change(semid,0,1);
	char * myfifo1="/tmp/al1";
	char * myfifo2="/tmp/al2";
	char * myfifo3="/tmp/al3";
	char * myfifo4="/tmp/al4";
	mkfifo(myfifo1,0666);
	mkfifo(myfifo2,0666);
	mkfifo(myfifo3,0666);
	mkfifo(myfifo4,0666);
	Fd[0].fd=open(myfifo1,O_RDONLY|O_NONBLOCK);
	Fd[1].fd=open(myfifo2,O_RDONLY|O_NONBLOCK);
	Fd[2].fd=open(myfifo3,O_RDONLY|O_NONBLOCK);
	Fd[3].fd=0;
	Fd[0].events=Fd[1].events=Fd[2].events=POLLIN;
	Fd[3].events= POLLIN | POLLRDBAND | POLLRDNORM | POLLPRI;
	rfd=open(myfifo4,O_WRONLY);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	sfd=1;
	printf("Polling \n");
	signal(SIGUSR1,toggler);
	while(1)
	{
		ret=poll(Fd,4,0);
		//printf("Pollinmg");
		fflush(stdout);
		if(ret>0)
		{
			for(i=0;i<4;i++)
			{
				if(Fd[i].revents & POLLIN)
				{
					if(Fd[i].fd==0)
						fflush(stdin);
					while((a=read(Fd[i].fd,buf,sizeof(buf)-1))>0)
					{
						buf[a]='\0';
						write(sfd,buf,a);
						fsync(sfd);
						if(Fd[i].fd==0)
							fflush(stdin);
						//fflush(stdout);
					}
				}
			}
		}
	}
}