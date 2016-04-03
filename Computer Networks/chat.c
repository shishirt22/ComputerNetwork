#include <stdio.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
struct pollfd Fd[2];
char buf[1000],*name,mess[1000];
int nfds,cfd,sfd;
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
	buf[i]='\0';
	return buf;
}
void * POLL(void * arg)
{
	int ret,a;
	while(1)
	{
		ret=poll(Fd,nfds,0);
		if(ret>0)
		{
			if(Fd[0].revents & POLLIN){
			printf("**********RECEIVED****************\n");
			while((a=read(Fd[0].fd,buf,sizeof(buf)))>0)
			{
				buf[a]='\0';
				printf("%s",buf);
			}
			printf("\n**********************************\n");
			}	
		}
	}
}
char * readLine()
{
	char * buf2=NULL;
	size_t bufsize;
	getline(&buf2,&bufsize,stdin);
	return buf2;
}
void * SCAN(void * arg)
{
	char * line;
	char mess[1000];
	while(1)
	{
		line=readLine();
	//	printf("Messgae to send : %s",line);
		strcpy(mess,name);
		strcat(mess," : ");
		strcat(mess,line);
		write(cfd,mess,strlen(mess));
	}
}
int main()
{
	int a,ret,pid;
	int fd1,fd2;
	fflush(stdout);
	char * myfifo1="/tmp/myfifo1";
	char * myfifo2="/tmp/myfifo2";
	char sfifo[20];
	char  cfifo[30];
	char temp[20];
	printf("Hello %d input your chat name\n",getpid());
	name=readLine();
	name[strlen(name)-1]='\0';
	fflush(stdin);
	//mkfifo(myfifo1,0666);
	//mkfifo(myfifo2,0666);
	strcpy(cfifo,"/tmp/");
	pid=getpid();
	strcat(cfifo,itoa(pid,temp,10));
	strcpy(mess,cfifo);
	strcat(mess,"*");
	strcat(mess,name);
	if(mkfifo(cfifo,0666)<0)
	{
		printf("failed to create pipe");
		perror("");
		return 0;
	}
	fd1=open(myfifo1,O_WRONLY);
	if(fd1<0)
	{
		printf("Fifo for Writing failed");
		perror("");
		return 0;
	}
	fd2=open(myfifo2,O_RDONLY);
	if(fd2<0)
	{
		printf("Fifo for Reading falied");
		perror("");
		return 0;
	}
	fflush(stdout);
	write(fd1,mess,sizeof(mess));
	read(fd2,sfifo,sizeof(sfifo));
//	printf("%s ",sfifo);
	fflush(stdout);
	if(sfifo[0]=='0')
	{
		printf("NO more connections allowed\n");
		close(fd1);
		close(fd2);
		unlink(cfifo);
		unlink(myfifo1);
		unlink(myfifo2);
		return 0;
	}
	else
	{
		sfd=open(sfifo,O_RDONLY|O_NONBLOCK);
		if(sfd<0)
		{
			perror("Fifo for message Reading:");
			return 0;
		}
		Fd[0].fd=sfd;
		Fd[0].events=POLLIN;
		nfds=1;
		 cfd=open(cfifo,O_WRONLY);
		 printf("Success !!!\nConnections Established With Server:\n");
		 fflush(stdout);
		if(cfd<0)
		{
			perror("");
			return 0;
		}
		//mkfifo(sfifo,0666);
		pthread_t T1,T2;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
		pthread_create(&T1,&attr,POLL,NULL);
		pthread_create(&T2,&attr,SCAN,NULL);
		pthread_join(T1,NULL);
		pthread_attr_destroy(&attr);
	}
}
