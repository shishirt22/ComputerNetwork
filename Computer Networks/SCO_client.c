#include <stdio.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "semaphore.h"
#include <pthread.h>
struct message_info{
	int sid;
	int client_id;
	char wfifo[50],rfifo[50];
};
struct message_info M;
int flag,sfd1,sfd2;
char buf[1000];
void stopper()
{
	printf("Server shut down\n");
	fflush(stdout);
	close(sfd1);
	close(sfd2);
	flag=0;
}
void * reading( void * arg)
{
	char * line;
	while(flag)
	{
		line=readLine();
		write(sfd1,line,strlen(line));
	}
	pthread_exit(NULL);
}
void * writing( void * arg)
{
	int a;
	while(flag)
	{
		a=read(sfd2,buf,sizeof(buf-1));
		if(a>0){
			buf[a]='\0';
			printf("%s",buf);
		}
	}
	pthread_exit(NULL);
}
int main()
{
	char * myfifo="/tmp/service_cord";
	char  * myfifo2="/tmp/service_cord2";
	int fd1,fd2,a;
	fd1=open(myfifo,O_WRONLY);
	fd2=open(myfifo2,O_RDONLY);
	signal(SIGPIPE,stopper);
	pthread_t reader,writer;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	while(1)
	{
		printf("Enter the id of server  whose services u want\n");
		scanf("%d",&a);
		M.sid=a;
		M.client_id=getpid();
		write(fd1,&M,sizeof(struct message_info));
		read(fd2,&M,sizeof(struct message_info));
		sfd1=open(M.wfifo,O_WRONLY);
		perror("wrinting");
		sfd2=open(M.rfifo,O_RDONLY|O_NONBLOCK);
		perror("reading");
		printf("%s  %s\n",M.wfifo,M.rfifo);
		flag=1;
		pthread_create(&reader,&attr,reading,NULL);
		pthread_create(&writer,&attr,writing,NULL);
		pthread_join(reader,NULL);
		pthread_join(writer,NULL);
	}
}