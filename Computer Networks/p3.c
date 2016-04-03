#include <stdio.h>
char ch[1000];
int main()
{
	printf("hp3");
	perror("prcess 3 start");
	size_t sbuf;
	//char * ch=NULL;
	//getline(&ch,&sbuf,stdin);
	scanf("%s",&ch);
	//read(0,ch,1000);
	printf("%sp3",ch);
	perror("process 3 end");
    return 0;
}
