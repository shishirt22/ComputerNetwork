#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
struct message{
	int cid;
	char servid[20];
	char pipeid[2][30];
};
char buf[1000];
char * readLine()
{
	char * buf2=NULL;
	size_t size2;
	getline(&buf2,&size2,stdin);
	return buf2;
}
struct pollfd fdr1[1],fdr[1];
int p1,p2,flag;
void handler()
{
	//printf("Closing files\n");
	//close(p1);
	close(p2);
	flag=0;
}
void * writer( void * arg)
{
	int a;
	char * line;
	while(flag)
	{
				line=readLine();
				//strcat(line," ");
				//line[strlen(line)-1]='\0';
				write(p2,line,strlen(line));
	}
	pthread_exit(NULL);
}
void * reader(void * arg)
{
	int a;
	char * line;
	int flag1=1;
	while(flag1)
	{
		a=poll(fdr,1,0);
		if(a>0)
		{
			if(fdr[0].revents & POLLIN)
			{
				while((a=read(fdr[0].fd,buf,sizeof(buf)-1))>0)
				{
					buf[a]='\0';
					printf("%s",buf);
					fflush(stdout);
				}
			}
			if(fdr[0].revents & POLLHUP && !(fdr[0].revents & POLLIN))
			{
				flag1=0;
			}
		}
	}
	close(fdr[0].fd);
	pthread_exit(NULL);
}
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
struct pollfd Fd[2];
int main()
{
	int a,ret;
	char * myfifo1="/tmp/server";
	int fd1=open(myfifo1,O_WRONLY);
	char name[20];
	struct message M;
	char temp[20];
	strcpy(name,"/tmp/");
	strcat(name,itoa(getpid(),temp,10));
	strcat(name,"a");
	strcpy(M.pipeid[0],name);
	strcpy(name,"/tmp/");
	strcat(name,itoa(getpid(),temp,10));
	strcat(name,"b");
	strcpy(M.pipeid[1],name);
	mkfifo(M.pipeid[0],0666);
	mkfifo(M.pipeid[1],0666);
	signal(SIGUSR1,handler);
	pthread_t Read,Write;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	flag=0;
	while(1)
	{
		printf("enter the name of service of u want\n");
		M.cid=getpid();
		scanf("%s",&(M.servid));
		write(fd1,&M,sizeof(struct message));
		fsync(fd1);
		flag=1;
		p1=open(M.pipeid[0],O_RDONLY|O_NONBLOCK);
		if(p1<0)
		{
			perror("");
		}
		p2=open(M.pipeid[1],O_WRONLY);
		fdr[0].fd=p1;
		fdr[0].events=POLLIN;
		pthread_create(&Read,&attr,reader,NULL);
		pthread_create(&Write,&attr,&writer,NULL);
		pthread_join(Read,NULL);
		pthread_join(Write,NULL);
	} 
}