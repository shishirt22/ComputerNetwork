#include <stdio.h>
char ch[100];
int main()
{
	sleep(5);
	freopen("2.txt","r",stdin);
	perror("");
	while(1)
	{
		scanf("%s",&ch);
		printf("%s ",ch);
		fflush(stdout);
		sleep(5);
	}
}