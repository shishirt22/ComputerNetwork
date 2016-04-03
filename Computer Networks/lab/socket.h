#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
int nonblock(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
int mkserver(int portno,int lcount,char * addr)
{
	struct sockaddr_in server;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	if(addr==NULL)
	{
		server.sin_addr.s_addr=INADDR_ANY;
	}
	else
	{
		printf("JJ");
		struct hostent * S=gethostbyname(addr);
		bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	}
	server.sin_port=htons(portno);
	if(lcount==0)
	{
		if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
			perror("CONNECTION FAILED");
		return sfd;
	}
	else
	{
		if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
		{
			perror("BIND FAILED");
		}
		listen(sfd,lcount);
		return sfd;
	}
}