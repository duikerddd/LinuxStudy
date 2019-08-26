//这是一个输出退出码demo`
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int i;
	for(i=0;i<255;++i)
	{
		//char *strerror(int errnum);
		//根据错误编号来获取错误信息
		//进程的退出码只用了一个字节存储,所以退出数据小于255
		//$? 返回上一个进程的返回值
		printf("error[%d]:%s\n",i,strerror(i));
	}
	return 0;
}
