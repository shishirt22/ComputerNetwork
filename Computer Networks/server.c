#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
struct message{
	int cid;
	char servid[20];
	char pipeid[2][30];
};
struct ACK{
int cid,p1,p2;
};
struct ACK A[10000];
void handler()
{
	printf("Waiting\n");
	int pid=waitpid(-1,0,WNOHANG);
	if(pid<0)
	{
		return ;
	}
	printf("wait ended\n");
	printf("CHILD ended %d\n",pid);
	kill(A[pid].cid,SIGUSR1);
	close(A[pid].p1);
	close(A[pid].p2);
}
struct pollfd Fd[2];
int main()
{
	char * myfifo1="/tmp/server";
	struct message M;
	int ret,a,p1,p2,pid;
	if(mkfifo(myfifo1,0666)<0)
	{
		perror("");
		//return 0;
	}
	int fd1=open(myfifo1,O_RDONLY|O_NONBLOCK);
	Fd[0].fd=fd1;
	Fd[0].events=POLLIN;
	signal(SIGCHLD,handler);
	while(1)
	{
		ret=poll(Fd,1,0);
		if(ret>0)
		{
			if(Fd[0].revents & POLLIN){
				printf("Request Received\n");
				read(Fd[0].fd,&M,sizeof(struct message));
				printf("%s %s\n",M.pipeid[0],M.pipeid[1]);
				p1=open(M.pipeid[0],O_WRONLY);
					if(p1<0)
					{
						perror("");
						continue;
					}
				p2=open(M.pipeid[1],O_RDONLY);
				if(p2<0)
				{
					perror("");
					continue;
				}
				pid=fork();
				if(pid==0)
				{
					//close(0);
					dup2(p2,0);
					//close(1);
					dup2(p1,1);
					execv(M.servid,NULL);
					perror("");
				}
				else
				{
					printf("pid %d\n",pid);
					A[pid].p1=p1;
					A[pid].p2=p2;
					A[pid].cid=M.cid;
				}
			}
		}
	}
} 