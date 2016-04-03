#include <stdio.h>
char ch[1000];
int main()
{
	freopen("e.txt","r",stdin);
	while(1)
	{
		scanf("%s",&ch);
		printf("%s",ch);
		sleep(10);
		fflush(stdout);
	}
}