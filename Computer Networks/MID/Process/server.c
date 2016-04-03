#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <signal.h>
#include  "markup.h"
int sfd[30],cfd[30],pid[30],E[30];
char buf[1000];
int cfds=0,pids=0;
void write_data(int fd)
{
	int a,j;
	while((a=read(fd,buf,sizeof(buf)))>0)
	{
		for(j=0;j<cfds;j++)
		{
			write(cfd[j],buf,a);
		}
	}
}
void handler1()
{
	if(cfds==0)
		return ;
	int fd=cfd[cfds-1];
	cfds--;
	pid[pids]=fork();
	if(pid[pids]==0)
	{
		dup2(fd,1);
		execv("E",NULL);
	}
	else
	{
		E[pids]=fd;
		pids++;
	}
}
void * handler2(int signum,siginfo_t * T,void * F)
{
	int i;
	for(i=0;i<pids;i++)
	{
		if(pid[i]==T->si_pid)
		{
			cfd[cfds]=E[i];
			cfds++;
		}
	}
}
int main(int arg,char * argc[])
{
	if(arg<2)
	{
		printf("Invalid Argument\n");
		return 0;
	}
	int fd[2],mfd,mfd2,nsfd,nfds,clilen;
	pipe(fd);
	perror("");
//	freopen("2.txt","r",stdin);
	int pid=fork();
	if(pid==0)
	{
		close(fd[0]);
		dup2(fd[1],1);
		execv("p1",NULL);
	}
	else
	{
		close(fd[1]);
		if(mkfifo("/tmp/hello",0666))
		{
			perror("FIFO 1");
		}
		if(mkfifo("/tmp/hello2",0666))
		{
			perror("FIFO2");
		}
		mfd=open("/tmp/hello",O_RDONLY|O_NONBLOCK);
		if(mfd<0)
			perror("OPEN 1");
		mfd2=open("/tmp/hello2",O_RDONLY|O_NONBLOCK);
		if(mfd2<0)
			perror("OPEN 2");
		pid=fork();
		if(pid==0)
		{
			close(mfd);
			close(mfd2);
			mfd=open("/tmp/hello",O_WRONLY);
			dup2(mfd,1);
			execv("p2",NULL);
			perror("FORK ERROR 2");
		}
		else
		{
			pid=fork();
			if(pid==0)
			{
				close(mfd2);
				close(mfd);
				mfd2=open("/tmp/hello2",O_WRONLY);
				dup2(mfd2,1);
				execv("p3",NULL);
				perror("FORK ERROR 3");
			}
		}
	}
	printf("FORKED SUCCESSFULLy");
	fd_set rfds;
	int i,j,mfds,ret,temp;
	struct sigaction act;
	act.sa_sigaction=handler2;
	act.sa_flags=SA_SIGINFO;
	sigaction(SIGCHLD,&act,NULL);
	sfd[0]=0;sfd[1]=mfd;
	sfd[2]=mfd2;
	sfd[3]=fd[0];
	signal(SIGUSR1,handler1);
	int portno=atoi(argc[1]);
	struct sockaddr_in server,client;
	nsfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(portno);
	if(bind(nsfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("BIND ERRORR");
		return 0;
	}
	struct timeval T;
	sfd[4]=nsfd;
	nfds=5;
	fg(0,sfd[3]);
	listen(nsfd,10);
	int flag=1;
	printf("SERVER RUNNIING\n");
	while(1)
	{
		T.tv_sec=0;
		T.tv_usec=500;
		if(flag==1){
			FD_ZERO(&rfds);
			mfds=-1;
			for(i=0;i<nfds-1;i++)
			{
				FD_SET(sfd[i],&rfds);
				mfds=mfds>sfd[i]?mfds:sfd[i];
			}
		}
		else{
			FD_CLR(sfd[4],&rfds);
		}
			FD_SET(sfd[4],&rfds);
			mfds=mfds>sfd[4]?mfds:sfd[4];
		/*for(i=0;i<cfds;i++)
		{
			FD_SET(cfd[i],&rfds);
			mfds=mfds>cfd[i]?mfds:cfd[i];
		}*/
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			if(cfds>=1)
			printf("printing\n");
			if(FD_ISSET(0,&rfds))
			{
				//printf("Reading stdin\n");
				if(cfds>0){
					write_data(0);
					flag=1;
				}
				else
					flag=0;
			}
			if(FD_ISSET(sfd[2],&rfds))
			{
				fflush(stdout);
				if(cfds>0&&flag){
				printf("READING 2");
					write_data(sfd[2]);
					flag=1;
				}
				else
					flag=0;
			}
			if(FD_ISSET(sfd[3],&rfds))
			{
				if(cfds>0&&flag){
					write_data(sfd[3]);
					flag=1;
				}
				else
					flag=0;
			}
			if(FD_ISSET(sfd[1],&rfds))
			{
				if(cfds>0&&flag){
					printf("READING from the file 1");
					fflush(stdout);
					write_data(sfd[1]);
					printf("RETURNED DROM 1");
					flag=1;
				}
				else
					flag=0;
			}
			if(FD_ISSET(sfd[4],&rfds))
			{
				printf("INCOMIG CONNECTION\n");
				clilen=sizeof(client);
				temp=accept(sfd[4],(struct sockaddr *)&client,&clilen);
				cfd[cfds]=temp;
				cfds++;
			}
		}
	}
}