//文件重定向和文件描述符规则demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>

int main()
{
	printf("stdin:%d\n",stdin->_fileno);
	printf("stdout:%d\n",stdout->_fileno);
	printf("stderr:%d\n",stderr->_fileno);
        close(1);
	int fd = open("./re.txt",O_CREAT | O_RDWR,0664);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	
	//int dup2(int oldfd,int newfd);
	//文件重定向,newfd指向oldfd所指向的文件
	//本该打印在显示器上的说明,写到文件里
	dup2(1,3);
	printf("fd:[%d]\n",fd);
	close(fd);
	return 0;
}
