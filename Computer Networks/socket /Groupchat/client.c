#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
int gfd;
char buf[1000];
void * Read(void * arg)
{
	int a;
	while(1)
	{
		a=read(gfd,buf,sizeof(buf));
		if(a>0)
		{
			buf[a]='\0';
			printf("%s",buf);
		}
	}
}
void * Write(void * arg)
{
	while(1)
	{
		scanf("%s",&buf);
		write(gfd,buf,strlen(buf));
	}
}
int main(int argn,char * argc[])
{
	if(argn<3)
	{
		printf("Invalid error\n");
		return 0;
	}
	int a;
	pthread_t reader,writer;
	struct sockaddr_in server;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	bzero((char *)&server,sizeof(server));
	struct hostent * s_name;
	s_name=gethostbyname(argc[1]);
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argc[2]));
	bcopy((char *)s_name->h_addr,(char *)&server.sin_addr.s_addr,s_name->h_length);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("CONNection FAILED");
		return 0;
	}
	printf("COnnection Established with main server");
	a=read(sfd,buf,sizeof(buf));
	buf[a]='\0';
	printf("%s",buf);
	
	while(1){
			scanf("%d",&a);
		if(a==0){
			write(sfd,"0",1);
			a=read(sfd,buf,sizeof(buf));
			buf[a]='\0';
			printf("%s",buf);
			fflush(stdout);
			struct sockaddr_in group;
			group.sin_family=AF_INET;
			group.sin_port=htons(atoi(buf));
			bcopy((char *)s_name->h_addr,(char *)&group.sin_addr.s_addr,s_name->h_length);
			printf("SUCCESS");
			write(sfd,"1",1);
			close(sfd);
			gfd=socket(AF_INET,SOCK_STREAM,0);
			if(connect(gfd,(struct sockaddr *)&group,sizeof(group)))
			{
				perror("CONNCTION FAILED WITH GRP");
			}
			else{
				printf("CONNECTION Established");
				pthread_create(&reader,NULL,Read,NULL);
				pthread_create(&writer,NULL,Write,NULL);
				pthread_join(reader,NULL);
			}
		}
		else
		{
			strcpy(buf,"");
			sprintf(buf,"%d",a);
			write(sfd,buf,strlen(buf));
			a=read(sfd,buf,sizeof(buf));
			buf[a]='\0';
			a=atoi(buf);
			if(a==0)
			{
				printf("NO such group exist");
			}
			else
			{
				struct sockaddr_in group;
				getdetails(&group,buf);
				write(sfd,"1",1);
				close(sfd);
				gfd=socket(AF_INET,SOCK_STREAM,0);
				if(connect(gfd,(struct sockaddr *)&group,sizeof(group)))
				{
					perror("CONNCTION FAILED WITH GRP");
				}
				else{
					printf("CONNECTION Established");
					pthread_create(&reader,NULL,Read,NULL);
					pthread_create(&writer,NULL,Write,NULL);
					pthread_join(reader,NULL);
				}
			}
		}
	}
}