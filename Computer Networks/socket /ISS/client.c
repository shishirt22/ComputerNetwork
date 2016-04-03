#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
int sfd;
char buf[1000],buf2[1000];
void sig(){
	perror("");
}
void * Read(void * arg)
{
	int a;
	while(1)
	{
		a=read(sfd,buf,sizeof(buf));
		if(a>0){
		buf[a]='\0';
		printf("%d %s\n",a,buf);
		}
	}
}
void * Write(void * arg)
{
	while(1)
	{
		scanf("%s",&buf2);
		strcat(buf2," ");
		write(sfd,buf2,strlen(buf2)+1);
	}
}
int main(int argn,char * argc[])
{

	if(argn<2)
	{
		printf("Invalid Argument\n");
		return 0;
	}
	struct sockaddr_in server;
	int portno;
	pthread_t reader,writer;
	struct hostent * s_name;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	portno=atoi(argc[2]);
	s_name=gethostbyname(argc[1]);
	if(s_name==NULL)
	{
		perror("NO SUCH SERVER");
		return 0;
	}
	bzero((char *)&server,sizeof(server));
	server.sin_family=AF_INET;
	bcopy((char *)s_name->h_addr, (char *)&server.sin_addr.s_addr,s_name->h_length);
	server.sin_port=htons(portno);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("CONNECTION FAILED\n");
	}
	printf("\t\tConnection Established\n");
	pthread_create(&reader,NULL,Read,NULL);
	pthread_create(&writer,NULL,Write,NULL);
	pthread_join(reader,NULL);
}