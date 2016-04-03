#include <stdio.h>
char ch[1000];
int main()
{
	printf("hp1");
	perror("process 1 start");
	scanf("%s",&ch);
	//lseek(0,0,SEEK_SET);
	//read(0,ch,1000);
	printf("%sp1 ",ch);
	fflush(stdout);
	perror("process 1 end");
    return 0;
}
