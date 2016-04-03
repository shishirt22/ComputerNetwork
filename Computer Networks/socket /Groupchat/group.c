#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int c[100];
int cl=0;
int main()
{
	struct sockaddr_in server,client;
	int client;
	struct timeval T;
	while(1)
	{
		FD_ZERO(&rfds);
		T.tv_sec=0;
		T.tv_usec=5000;
		mfds=0;
		FD_SET(0,&rfds);
		for(i=0;i<cl;i++)
		{
			FD_SET(c[i],&rfds);
			mfds=mfds>c[i]?mfds:c[i];
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T);
		if(ret>0){
			for(i=0;i<cl;i+++)
			{
				if(FD_ISSET(c[i],&rfds))
				{
					while((a=read(c[i],buf,sizeof(buf)))>0){
						buf[a]='\0';
						for(j=0;j<cl;j++)
						{
							if(j!=i)
							{
								write(c[j],buf,strlen(buf));
							}
						}
					}
				}
			}
				if(FD_ISSET(0,&rfds)){
				clilen=sizeof(client);
				c[cl]=accept(0,(struct sockaddr *)&client,&clilen);
				cl++;	
			}
		}
	}
}