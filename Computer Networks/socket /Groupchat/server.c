#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <fcntl.h>
void getdetails()
{
	strcpy(buf,"");
	for(i=0;i<gfd;i++)
	{
		sprintf(tmp," %d",P[i]);
		strcat(buf,tmp);
	}
}
int main(int arg,char* argc[])
{
	struct sockaddr_in server,client;
	int portno=atoi(argc[1]);
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(portno);
	fd_set rfds;
	int mfds=-1;
	if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("BIND");
	}
	listen(5);
	while(1)
	{
		clilen=sizeof(client);
		cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
		getdetails();
		write(cfd,buf,sizeof(buf));
		a=read(cfd,buf,sizeof(buf));
		buf[a]='\0';
		a=atoi(buf);
		if(a==0)
		{
			printf("New group");
			lsfd=socket(AF_INET,SOCK_STREAM,0);
			server.sin_family=AF_INET;
			server.sin_addr.s_addr=INADDR_ANY;
			server.sin_port=htons(portno);
			P[gfd]=portno;
			if(bind(lsfd,(struct sockaddr *)&server,sizeof(server)))
			{
				perror("BIND ERROR");
			}
			listen(lsfd,8);
			sprintf(buf,"%d",portno);
			write(cfd,buf,strlen(buf));
			pid=fork();
			if(pid==0)
			{
				close(sfd);
				dup2(lsfd,0);
				execv("g",NULL);
				perror("FORK ERRROR");
			}
			gfd++;
			portno++;
		}
		else
		{
			close(cfd);
		}
	}
}