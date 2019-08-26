//进程等待demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	int pid=fork();
	if(pid==0)
	{
		printf("do you wait me?\n");
		sleep(3);
	}
	else
	{
	//pid_t wait(int *status);
	//等待任意一个子进程退出
	//status : 输出型参数,用于获取进程退出返回值
	int pid = wait();	
	printf("yes , i love you\n");
	}
	return 0;
}
