#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
int main()
{
	int fd;
	fd=open("hello.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
	close(1);
	int dupfd=dup(fd);
	printf("ggt");
	fflush(stdout);
	if(close(fd)!=0)
		printf("error\n");
	//perror("")
	write(dupfd,"mess",4);
	if(close(dupfd!=0))
		printf("hello\n");
}