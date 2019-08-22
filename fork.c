//父子进程创建demo
#include<stdio.h>

int main()
{
	int pid=fork();
	if(pid==0)
	{
		printf("i am child,pid:[%d]\n",getpid());
	}
	else
	{
		printf("i am your father,pid:[%d]\n",getpid());
	}

	while(1)
	{
		sleep(1);
	}
	return 0;
}
