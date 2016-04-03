#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
union semun{
	int val;
	struct semid_ds * buf;
	unsigned short * array;
};
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
	buf[i]='a';
	buf[i+1]='\0';
	return buf;
}
char * readLine()
{
	char * buf2=NULL;
	size_t bufsize;
	getline(&buf2,&bufsize,stdin);
	return buf2;
}
void setval(int semid,int semnum,int semval)
{
	union semun arg;
	arg.val=semval;
	semctl(semid,semnum,SETVAL,semval);
}
void change(int semid,int semnum,int semval)
{
	struct sembuf sem_lock={semnum,semval,0};
	semop(semid,&sem_lock,1);
}