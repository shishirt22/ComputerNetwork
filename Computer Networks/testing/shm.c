#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
struct SHM{
	int sid,co_id,serv_id,cl_id;
	char wfifo[50],rfifo[50];
	int availablity;
};
struct SHM * S;
int main()
{
	int shmid=shmget(IPC_PRIVATE,20*sizeof(struct SHM),IPC_CREAT|0666);
	S=(struct SHM *)shmat(shmid,NULL,0);
	S[0].sid=1;
	S[0].co_id=2;
	strcpy(S[0].wfifo,"hello");
	printf("%d %d %s\n",S[0].sid,S[0].co_id,S[0].wfifo);
	S[1].sid=S[0].sid;
	S[1].co_id=S[0].co_id;
	printf("%d %d \n",S[1].sid,S[1].co_id);
	shmdt(S);
	shmctl(shmid,IPC_RMID,NULL);
}