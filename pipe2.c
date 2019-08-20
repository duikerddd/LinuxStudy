// ls | grep 的模拟实现demo
#include<stdio.h>
#include<unistd.h>
int main()
{
	int pipefd[2];
	int ret = pipe(pipefd);
	if(ret<0)
	{
		perror("pipe error");
		return -1;
	}

	int pid1 = fork();
	if(pid1 == 0)
	{
		//ls 标准输出重定向写入到pipefd[1]
		close(pipefd[0]);
		dup2(pipefd[1],1);
		execlp("ls","ls",NULL);
		_exit(0);
	}
	int pid2 = fork();
	if(pid2 == 0)
	{
		//grep 标准输入重定向读取pipefd[0]
		close(pipefd[1]);
		dup2(pipefd[0],0);
		execlp("grep","grep","p",NULL);
		_exit(0);	
	}	

	close(pipefd[0]);
	close(pipefd[1]);
	wait(NULL);
	wait(NULL);
	return 0;
}
