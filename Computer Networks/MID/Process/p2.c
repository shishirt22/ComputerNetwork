#include <stdio.h>
char ch[1000];
int main()
{
	freopen("2.txt","r",stdin);
	while(1)
	{
		scanf("%s",&ch);
		printf("%s",ch);
		sleep(10);
		fflush(stdout);
	}
}