#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
char buf[100];
int main(int arg,char * argc[])
{
	if(arg<2)
	{
		printf("Error!!! Invalid agument");
		return 0;
	}
	int sfd,nsfd,n,clilen;;
	struct sockaddr_in serv,cliserv;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	bzero((char *)&serv,sizeof(serv));
	int portno=atoi(argc[1]);
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=INADDR_ANY;
	serv.sin_port=htons(portno);
	if(bind(sfd,(struct sockaddr *)&serv,sizeof(serv)))
	{
		printf("bind failed\n");
		return 0;
	}
	listen(sfd,5);
	clilen=sizeof(cliserv);
	nsfd=accept(sfd,(struct sockaddr *)&serv,&clilen);
	if(nsfd<0)
	{
		perror("NSFD ");
		return  0;
	}
	while(1)
	{
		n=read(nsfd,buf,sizeof(buf));
		buf[n]='\0';
		printf("%s\n",buf);
		scanf("%s",buf);
		write(nsfd,buf,strlen(buf));
	}
}