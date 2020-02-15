//子进程返回值(status)的解析demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

	int status;
	int pid=fork();
	if(pid == 0)
	{
	
		sleep(5);
		return 5;
	}
	else
	{
		//waitpid : pid_t waitpid(pid_t pid,int *status,int options);
		//pid :
		//-1	等待任意子进程退出
		//>0	等待指定子进程退出
		//status : 输出型参数,用于获取子进程退出返回值
		//options :
		//0	默认等待子进程退出
		//WNOHANG	设置为非阻塞,如果没有子进程已经退出,则立即返回
		//返回值:
		//	>0 退出子进程pid
		//	==0 没有子进程退出
		//	<0	出错
		while(waitpid(pid,&status,0) == 0)
		{
			printf("---\n");
		}
		//00000000 233 0 0000000 低七位为0正常
		if((status & 0x7f)==0)
		{
			printf("exit code :[%d]\n",status>>8 & 0xff);//计算退出码的值,十进制输出
		}
	       	//封装上面的方法
		// if(WIFEXITD(status))
        	// {
	        //  printf("exit code:%d\n:",WEXITSTATUS(status));
	        //}     
	}
	return 0;

}
