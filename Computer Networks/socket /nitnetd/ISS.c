#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
struct Protocol{
	char path[20];
	int portno;
	int limit;
	int no;
};
fd_set rfds;
struct timeval T;
struct Protocol P[4];
int Pid[1000],E[1000],D[5];
int pids=0;
void handler2(int signum,siginfo_t * T,void * F)
{
	int i;
	for(i=0;i<pids;i++)
	{
		if(Pid[i]==T->si_pid)
		{
			P[E[i]].no--;
			if(P[E[i]].no!=P[E[i]].limit)
				D[E[i]]=1;
			break;
		}
	}
}
int main(int argn,char * argc[])
{
	int i;
	freopen("init.txt","r",stdin);
	perror("");
	for(i=0;i<4;i++)
	{
		scanf("%s %d %d",&(P[i].path),&(P[i].portno),&(P[i].limit));
		printf("%s %d %d",P[i].path,P[i].portno,P[i].limit);
		P[i].no=0;
	}
	struct  sockaddr_in server[4],client;
	int portno,clilen,sfd[4],cfd,mfds,ret;
	struct sigaction act;
	act.sa_sigaction=handler2;
	act.sa_flags=SA_SIGINFO;
	sigaction(SIGCHLD,&act,NULL);
	//memset(&client,0,sizeof(client));
	//portno=atoi(argc[1]);
	for(i=0;i<4;i++){
		D[i]=1;
		sfd[i]=socket(AF_INET,SOCK_STREAM,0);
		bzero(&server[i],sizeof(server[i]));
		server[i].sin_family=AF_INET;
		server[i].sin_addr.s_addr=INADDR_ANY;
		server[i].sin_port=htons(P[i].portno);
		if(bind(sfd[i],(struct sockaddr *)&server[i],sizeof(server[i])))
		{
			perror("BIND FAILED");
			return 0;
		}
		else
		{
			printf("Successs");
		}
		listen(sfd[i],10);
	}
	int pid,j;
	clilen=sizeof(client);
	printf("\t\tSERVER RUNNING\n");
	while(1)
	{
		mfds=-10;
		T.tv_sec=0;
			T.tv_usec=5;
		for(i=0;i<4;i++)
		{
			if(D[i]==1){
			FD_CLR(sfd[i],&rfds);
			FD_SET(sfd[i],&rfds);
			}
			mfds=mfds<sfd[i]?sfd[i]:mfds;
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			//perror("Connection Coming");
			for(i=0;i<4;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{
					if(P[i].no>=P[i].limit){
						D[i]=0;
						printf("wait");
					}
					else{
						bzero(&client,sizeof(client));
						clilen=sizeof(client);
						cfd=accept(sfd[i],(struct sockaddr *)(&client),&clilen);
						E[pids]=i;
						Pid[pids]=fork();
						if(Pid[pids]==0)
						{
							for(j=0;j<4;j++)
								close(sfd[j]);
							dup2(cfd,0);
							dup2(cfd,1);
							execv(P[i].path,NULL);
							perror("EXEC ERROR");
						}
						else
						{
							close(cfd);
							pids++;
							P[i].no++;
						}
					}
				}
			}
		}
	}
}