#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
int clilen,i,j,is_free[3],pid[3],port[3];
char buf[1000];
void handler(int signum,siginfo_t * T,void * F)
{
	for(i=0;i<3;i++)
	{
		if(pid[i]==T->si_pid)
		{
			printf("PLATFROM %d is free\n",i+1);
			fflush(stdout);
			is_free[i]=1;
		}
	}
}
int main(int argn,char * argc[])
{
	if(argn<2)
	{
		printf("INAVALID ARGUMENT!!");
		return 0;
	}
	int portno=atoi(argc[1]);
	int pfd[3],tfd[3];
	struct sockaddr_in P[3],T[3],client;
	struct sigaction act;
	act.sa_sigaction=handler;
	act.sa_flags=SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);
	for(i=0;i<3;i++)
	{
		pfd[i]=socket(AF_INET,SOCK_STREAM,0);
		bzero(&P[i],sizeof(P[i]));
		P[i].sin_family=AF_INET;
		P[i].sin_addr.s_addr=INADDR_ANY;
		P[i].sin_port=htons(portno);
		port[i]=portno;
		if(bind(pfd[i],(struct sockaddr *)&P[i],sizeof(P[i])))
			perror("BIND ERROR");
		listen(pfd[i],5);
		portno++;
		is_free[i]=1;
	}
	for(i=0;i<3;i++)
	{
		tfd[i]=socket(AF_INET,SOCK_STREAM,0);
		bzero(&T[i],sizeof(T[i]));
		T[i].sin_family=AF_INET;
		T[i].sin_addr.s_addr=INADDR_ANY;
		T[i].sin_port=htons(portno);
		if(bind(tfd[i],(struct sockaddr *)&T[i],sizeof(T[i]))<0)
			perror("BIND ERROR");
		listen(tfd[i],5);
		printf("%d\n",portno);
		portno++;
	}
	pid[0]=fork();
	if(pid[0]==0)
	{
		close(pfd[1]);
		close(pfd[2]);
		close(tfd[0]);
		close(tfd[1]);
		close(tfd[2]);
		dup2(pfd[0],0);
		execv("p1",NULL);
		perror("FORK ERROR");
	}
	else
	{
		close(pfd[0]);
		pid[1]=fork();
		if(pid[1]==0)
		{
			close(pfd[2]);
			close(tfd[0]);
			close(tfd[1]);
			close(tfd[2]);
			dup2(pfd[1],0);
			execv("p2",NULL);
			perror("FORK ERROR");
		}
		else
		{
			close(pfd[1]);
			pid[2]=fork();
			if(pid[2]==0)
			{
				close(tfd[0]);
				close(tfd[1]);
				close(tfd[2]);
				dup2(pfd[2],0);
				execv("p3",NULL);
				perror("FORK ERROR");
			}
			else
				close(pfd[2]);
		}
	}
	struct timeval T1;
	fd_set rfds;
	printf("SERVER RUNNING\n");
	int flag=1,ret,station,mfds,cfd;
	while(1)
	{
		T1.tv_sec=0;
		T1.tv_usec=0;
		if(flag==1){
			mfds=-1;FD_ZERO(&rfds);
			for(i=0;i<3;i++)
			{
				FD_SET(tfd[i],&rfds);
				mfds=mfds>tfd[i]?mfds:tfd[i];
			}
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T1);
		if(ret>0)
		{
			//printf("INCOMING CONNECTIOn\n");
			flag=0;
			for(i=0;i<3;i++)
			{
				if(is_free[i])
				{
					flag=1;
					printf("FRee found\n");
					station=i;
					break;
				}
			}
			fflush(stdout);
			if(flag)
			{
				clilen=sizeof(client);
				for(i=0;i<3;i++)
				{
					if(FD_ISSET(tfd[i],&rfds))
					{
						printf("TFD found\n");
						is_free[station]=0;
						cfd=accept(tfd[i],(struct sockaddr *)&client,&clilen);
						sprintf(buf,"%d",port[station]);
						write(cfd,buf,strlen(buf));
						close(cfd);
						for(j=i+1;j<3;j++)
						{
							if(FD_ISSET(tfd[j],&rfds))
							{
								flag=0;
								break;
							}
						}
						if(j==3)
						{
							flag=1;
						}
						break;
					}
				}
			}
		}
	}
}
