#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
	int fd=open("./test.fifo",O_RDONLY);
	while(1)
	{
		char buff[1024]={0};
		int ret=read(fd,buff,1023);
		if(ret>0)
		{
		printf("client say:%s\n",buff);
		}
		else if(ret == 0)
		{
		printf("write close!\n");
		return -1;
		}
		else 
		{
		perror("read error");
		return -1;
		}
	}	
	close(fd);
	return 0;
}
