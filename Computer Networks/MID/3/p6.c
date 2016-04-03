#include <stdio.h>
char  buf[1000];
int main()
{
	//freopen("1.txt","w",stdout);
	int a;
	while(1){
	while((a=read(0,buf,sizeof(buf)))>0)
	{
		buf[a]='\0';
		printf(" pROCESS %s",buf);
		fflush(stdout);
	}
	printf("WAIT");
	}
}