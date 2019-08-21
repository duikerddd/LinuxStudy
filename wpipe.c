//命名管道基本使用demo
//   int mkfifo(cosnt char* pathname,mode_t mode)
//   mode : 权限 返回值: 成功0  失败-1
//可见于文件系统创建一个管道文件,同一机器上任意进程间通信
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>

int main()
{
	int ret = mkfifo("./test.fifo",0664);
	if(ret<0)
	{
		perror("mkfifio error");
		return -1;
	}
	
	int fd = open("./test.fifo",O_WRONLY);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	printf("open fifo success!\n");

	while(1)
	{
		char buff[1024] = {0};
		scanf("%s",buff);
		write(fd,buff,strlen(buff));
		printf("buff:[%s]\n",buff);
	}        
	close(fd);
	return 0;
}
