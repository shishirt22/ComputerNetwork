#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "../semaphore.h"

struct SHM{
	int  pid;
};
struct SHM * S;
int main()
{
	int key=ftok(".",'e');
	int shmid=shmget(key,sizeof(struct SHM),0666);
	perror("semget");
	S=(struct SHM *)shmat(shmid,NULL,0);
	perror("attch");
	key=ftok(".",'b');
	int semid=semget(key,1,0666);
	perror("sem");
	change(semid,0,-1);
	int pid=S[0].pid;
	int a;
	printf("%d \n",pid);
	while(1)
	{
		printf("Press 1 to send signal\n");
		scanf("%d",&a);
		if(a==1)
		{
			kill(pid,SIGUSR1);
		}
		else
		{
			printf("Wrong key pressed\n");
		}
	}
}