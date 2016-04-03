#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char * itoa(int a,char * buf,int base)
{
	char temp[20];
	int c=0,i=0;
	while(a>0)
	{
		temp[c]=(char)((a%base)+'0');
		a=a/base;
		c++;
	}
	c--;
	while(c>=0)
	{
		buf[i]=temp[c];
		c--;
		i++;
	}
	buf[i]='\0';
	//printf("%s",buf);
	return buf;
}
int main()
{
	char buf[20],a[20],b[20];
	int A,B;
	setbuf(stdout,NULL);
	printf("Enter The two numbers you want to add\n");
	//fsync(1);
	printf("Enter the first number\n");
//	fflush(stdout);
	scanf("%s",&a);
//	printf("%s",a);
	printf("Enter the second\n");
//	fflush(stdout);
	scanf("%s",&b);
	A=atoi(a);
	B=atoi(b);
	//itoa(A+B,buf,10);
	printf("Result %s",itoa(A+B,buf,10));
//	fsync(1);
//	fflush(stdout);
}