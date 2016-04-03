#include <stdio.h>
#include <fcntl.h>
#include <string.h>

char buf[1000];
int main()
{
	char * myfifo="/tmp/al4";
	int a;
	int fd=open(myfifo,O_RDONLY);
	while(1){
		while((a=read(fd,buf,sizeof(buf)-1))>0)
		{
			buf[a]='\0';
			printf("%s\n",buf);
		}
	}
}