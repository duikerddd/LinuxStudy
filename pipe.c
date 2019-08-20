//匿名管道基本使用demo
#include<stdio.h>
#include<string.h>
#include<errno.h>

int main()
{
	//匿名管道需要在子进程之前创建
	//int pipe(int pipefd[2]);
	//pipefd : 用于获取管道中的操作描述符
	//pipefd[0] : 从管道中读取数据
	//pipefd[1] : 从管道中写入数据
	int pipefd[2]={0};
	int ret = pipe(pipefd);
	if(ret<0)
	{
		perror("pipe error");
		return -1;
	}

	int pid = fork();
	if(pid < 0)
	{
		perror("pipe error");
		return -1;
	}	
	else if(pid == 0)
	{
		//子进程写入数据
		close(pipefd[0]);
		char* ptr = "do you like me?";
		write(pipefd[1],ptr,strlen(ptr));
	}
	else
	{
		//父进程读取数据
		close(pipefd[1]);
		char buff[1024]={0};
		read(pipefd[0],buff,sizeof(buff)-1);
		printf("read buff : %s\n",buff);
	}
	
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}
