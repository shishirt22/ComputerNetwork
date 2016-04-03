#include <stdio.h>
char ch[100];
int main()
{
	freopen("1.txt","r",stdin);
	while(1)
	{
		scanf("%s",&ch);
		printf("%s",ch);
		sleep(10);
		fflush(stdout);
	}
}