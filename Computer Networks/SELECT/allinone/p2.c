#include <stdio.h>
#include <fcntl.h>
#include <string.h>
char * readLine()
{
	char * buf2=NULL;
	size_t bufsize;
	getline(&buf2,&bufsize,stdin);
	return buf2;
}
int main()
{
	char * myfifo="/tmp/al2";
	int fd1=open(myfifo,O_WRONLY);
	char * line;
	while(1)
	{
		line=readLine();
		write(fd1,line,strlen(line));
	}
}