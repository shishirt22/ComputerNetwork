#include <stdio.h>
char ch[1000];
int main()
{
	printf("hp2");
	perror("process 2 start ");
//	printf("%d",stdin);
	fflush(stdout);
	scanf("%s",&ch);
	//char * ch=NULL;
	//size_t sbuf;
	//read(0,ch,1000);
	//getline(&ch,&sbuf,stdin);
	printf("%sp2 ",ch);
	//fflush(1);
	perror("process 2 end");
    return 0;
}
