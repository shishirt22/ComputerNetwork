#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/time.h>
char buf[1000];
struct Protocols{
	char path[100];
	int portno;
	int active;
	int sfd;
};
int nfds;
struct Protocols P[100];
void handler(int signum,siginfo_t * T,void * F)
{
	int i;
	for(i=0;i<nfds;i++)
	{
		if(P[i].active==T->si_pid)
		{
			P[i].active=0;
		}
	}
}
int main(int arg,char * argc[])
{
	//freopen("inetd.txt","r",stdin);
	mkfifo("/tmp/inetd",0666);
	int ifd=open("/tmp/inetd",O_RDONLY);
	perror("");
	int i,ret;
	int ap=read(ifd,buf,sizeof(buf));
	buf[ap]='\0';
	strcat(buf," ");
	char * temp;
	temp=strtok(buf," \n\t");
	i=0;
	printf("%s\n",temp);
	fflush(stdout);
	while(temp!=NULL)
	{
		strcpy(P[i].path,temp);
		temp=strtok(NULL," \n\t");
		P[i].portno=atoi(temp);
		printf("%s %d\n",P[i].path,P[i].portno);
		temp=strtok(NULL," \n\t");
		i++;
		P[i].active=0;
	}
	//printf("%s",buf);
	//read(0,buf,sizeof(buf));
	//sleep(2);
	nfds=i;
	struct sockaddr_in server[4],client;
	int j,clilen,mfds,pid;
	for(i=0;i<nfds;i++)
	{
		P[i].sfd=socket(AF_INET,SOCK_STREAM,0);
		bzero(&server[i],sizeof(server[i]));
		server[i].sin_family=AF_INET;
		server[i].sin_addr.s_addr=INADDR_ANY;
		server[i].sin_port=htons(P[i].portno);
		if(bind(P[i].sfd,(struct sockaddr *)&server[i],sizeof(server[i])))
		{
			perror("BIND ERROR");
			return 0;
		}
		listen(P[i].sfd,10);
	}
//	nfds=4;
	fd_set rfds;
	struct timeval T;
	while(1)
	{
		T.tv_sec=0;
		T.tv_usec=5000;
		mfds=ifd;
		FD_ZERO(&rfds);
		FD_SET(ifd,&rfds);
		for(i=0;i<nfds;i++)
		{
			if(P[i].active==0){
				FD_SET(P[i].sfd,&rfds);
				mfds=mfds>P[i].sfd?mfds:P[i].sfd;
			}
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0)
		{
			printf("Incominmg");
			fflush(stdout);
			for(i=0;i<nfds;i++)
			{
				if(FD_ISSET(P[i].sfd,&rfds))
				{
					printf("Incoming connection");
					fflush(stdout);
					P[i].active=fork();
					if(P[i].active==0)
					{
						for(j=0;j<nfds;j++)
						{
							if(i==j)
								continue;
							else
								close(P[j].sfd);
						}
						dup2(P[i].sfd,0);
						execv(P[i].path,NULL);
						perror("FORK ERROR\n");
					}
				}
			}
			if(FD_ISSET(ifd,&rfds))
			{
				printf("Changes in netd\n");
				read(ifd,buf,sizeof(buf));
				temp=strtok(buf," \n\t");
				while(temp!=NULL)
				{
					strcpy(P[nfds].path,temp);
					temp=strtok(NULL," \n\t");
					P[i].portno=atoi(temp);
					temp=strtok(NULL," \n\t");
					printf("%s %d\n",P[nfds].path,P[nfds].portno);
					P[nfds].active=0;
					P[nfds].sfd=socket(AF_INET,SOCK_STREAM,0);
					bzero(&server[0],sizeof(server[0]));
					server[0].sin_family=AF_INET;
					server[0].sin_addr.s_addr=INADDR_ANY;
					server[0].sin_port=htons(P[i].portno);
					if(bind(P[nfds].sfd,(struct sockaddr *)&server[0],sizeof(server[0])))
					{
						perror("BIND ERROR");
					}
					listen(P[nfds].sfd,10);
					nfds++;
				}
				//printf("%d",scanf("%s %d",&(P[nfds].path),&(P[nfds].portno)));
			}
		}
	}
}