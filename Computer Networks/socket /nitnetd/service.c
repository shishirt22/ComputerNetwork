#include <stdio.h>
#include <string.h>
int main()
{
	int a;
	char  buf[1000];
	while(1)
	{
		a=read(0,buf,sizeof(buf));
			buf[a]='\0';
		strcat(buf,"Received");
		write(1,buf,strlen(buf));
	}
}