#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#define Lim 10
struct clients{
	char name[50];
};
struct clients C[10];
struct pollfd Fd[Lim];
int cfd[Lim];
char buf[1000],mess[1000];
char * itoa(int a,char * buf,int base)
{
	char temp[20];
	int c=0,i=0;
	while(a>0)
	{
		temp[c]=(char)((a%base)+'0');
		a=a/base;
		c++;
	}
	c--;
	while(c>=0)
	{
		buf[i]=temp[c];
		c--;
		i++;
	}
	buf[i]='a';
	buf[i+1]='\0';
	return buf;
}
int main()
{
	int ret,a,i,j,newfd;
	char * token1,* token2;
	char  name[30];
	strcpy(name,"/tmp/");
	char temp[3];
	int nfds;
	char * myfifo1="/tmp/myfifo1";
	if(mkfifo(myfifo1,0666)<0)
	{
	//	printf("First exist");
		//fflush(stdout);
	//	perror("");
	}
	int fd=open(myfifo1,O_RDONLY|O_NONBLOCK);
	if(fd<0)
	{
		perror("");
		return 0;
	}
	Fd[0].fd=fd;
	Fd[0].events=POLLIN;
	char * myfifo2="/tmp/myfifo2";
	//printf("%s",myfifo2);
	//fflush(1);
	if(mkfifo(myfifo2,0666)<0)
	{
		//close(fd);
		//unlink(myfifo1);
		//printf("Second exist");
		fflush(stdout);
	//	perror("");
	//	fflush(stderr);
		//return 0;
	}
	int fd2=open(myfifo2,O_WRONLY);
	if(fd2<0)
	{
		perror("");
		return 0;
	}
	fflush(stdout);
	//signal(SIGPIPE,sig);
	//Fd[1].fd=fd2;
	//Fd[0].events=POLLOUT;
	int lim=0;
	nfds=1;
	while(1)
	{
		ret=poll(Fd,nfds,0);
		if(ret>0)
		{
			if(Fd[0].revents & POLLIN)
			{
				if(lim<Lim)
				{
					printf("New Connections established");
					read(Fd[0].fd,buf,sizeof(buf));
					token1=strtok(buf,"*");
					token2=strtok(NULL,"*");
					strcpy(buf,token1);
					strcpy(C[lim].name,token2);
					printf(" with pipe id %s\n",buf);
					fflush(stdout);
					strcpy(mess,C[lim].name);
					strcat(mess," has joined the conversation\n");
					printf("%s \n",mess);
					for(j=0;j<lim;j++)
					{
						write(cfd[j],mess,strlen(mess));
					}
					//if(mkfifo(buf,0666)<0)
					strcat(name,itoa(lim,temp,10));
					if(mkfifo(name,0666)<0)
					{
						//perror("");
						//printf("Error");
					//	fflush(stdout);
					//	continue;
					}
					if(write(fd2,name,strlen(name)+1)<0)
						//perror("");
					//close(fd2);
					//fd2=open(myfifo2,O_WRONLY);
					if(fd2<0)
					{
						printf("Heleo");
						fflush(stdout);
					}
				//	fflush(stderr);
				//	fflush(fd2);
					cfd[lim]=open(name,O_WRONLY);
					newfd=open(buf,O_RDONLY|O_NONBLOCK);
					if(newfd<0)
					{
						//perror("");
						continue;
					}
					Fd[nfds].fd=newfd;
					Fd[nfds].events=POLLIN;
					nfds++;
					strcpy(name,"/tmp/");
					lim++;
				}
				else
				{
					write(fd2,"0",2);
					//fflush(fd2);
				}
			}
			else
			{
				for(i=1;i<nfds;i++)
				{
					if(Fd[i].revents & POLLIN)
					{
						while((a=read(Fd[i].fd,buf,sizeof(buf)))>0)
						{
							for(j=0;j<lim;j++)
							{
								if(i==j+1)
								{
									continue;
								}
								else
									write(cfd[j],buf,a);
	//							printf("%d\n",a);
								fflush(stdout);
							}
						}
					}
					else if(Fd[i].revents & POLLHUP)
					{
						int ptr,sptr=1;
						j=0;
						strcpy(mess,C[i-1].name);
						strcat(mess," has left the conversation\n");
						for(ptr=1;ptr<nfds;ptr++)
						{
							if(ptr!=i)
							{
								Fd[sptr].fd=Fd[ptr].fd;
								Fd[sptr].events=Fd[ptr].events;
								Fd[sptr].revents=Fd[ptr].revents;
								cfd[j]=cfd[ptr-1];
								write(cfd[j],mess,strlen(mess));
								strcpy(C[j].name,C[ptr-1].name);
								sptr++;
								j++;
								continue;
							}
							else
							{
								close(Fd[ptr].fd);
								close(cfd[ptr-1]);
							}
						}
						nfds--;
						lim--;
						printf("%d Left The coverstaion\n",i);
					}
				}
			}
		}
	}
	unlink(myfifo1);
	unlink(myfifo2);
}
