#include <stdio.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
struct pollfd Fd[2];
void handler()
{
	printf("called handler");
	if(Fd[0].revents==POLLIN)
	{
		printf("raed datee\n");
	}
	exit(0);
}
char buf[1000];
char * readLine()
{
	char * buf2=NULL;
	size_t size2;
	getline(&buf2,&size2,stdin);
	return buf2;
}
int mains()
{
	int ret,a,pid;
	char * myfifo="/tmp/test";
	char * myfifo2="/tmp/test2";
	if(mkfifo(myfifo,0666)<0)
	{
		perror("");
	}
	if(mkfifo(myfifo2,0666)<0)
	{
		perror("");
	}
	int fd,fd2;
	pid=fork();
	if(pid==0)
	{
	//	close(fd);
    //close(fd2);
    fd2=open(myfifo2,O_WRONLY);
		fd=open(myfifo,O_RDONLY);
		dup2(fd,0);
		dup2(fd2,1);
		execv("/home/shishir/Documents/Computer Networks/testing/./test1",NULL);
		perror("");
		exit(0);

	}
	else
	{
	fd2=open(myfifo2,O_RDONLY|O_NONBLOCK);
	Fd[1].fd=fd2;
	Fd[1].events=POLLIN;
	 fd=open(myfifo,O_WRONLY);
	Fd[0].fd=fd;
	Fd[0].events=POLLOUT;
		char * line;
		signal(SIGCHLD,handler);
		printf("Polling\n");
		fflush(stdout);
		while(1)
		{
			ret=poll(Fd,2,0);
			if(ret>0)
			{
				if(Fd[0].revents & POLLOUT)
				{
					printf("Need output*************\n");
					line=readLine();
					//strcat(line," ");
					write(Fd[0].fd,line,strlen(line));
				}
				if(Fd[0].revents&POLLERR)
                    printf("ERRONR");
				if(Fd[0].revents&POLLHUP)
				{
					printf("Ended\n");

				}
				if(Fd[1].revents & POLLIN)
				{
					printf("Reading**************\n");
					while((a=read(Fd[1].fd,buf,sizeof(buf)-1))>0)
					{
						buf[a]='\0';
						printf("%s",buf);
					}
				}
			}
		}
	}
}
