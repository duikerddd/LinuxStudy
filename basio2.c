//系统io调用接口demo
// open	 close  read  write  lseek
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>

int main()
{
  umask(0);
  char* file="text.tmp";
  int fd = open(file,O_RDWR | O_CREAT,0777);
  if(fd<0)
  {
    perror("open error");
    return -1;
  }
  char* str = "hello \n";
  write(fd,str,strlen(str));

  lseek(fd,0,SEEK_SET);

  char buff[1024]={0};
  read(fd,buff,sizeof(buff)-1);
  printf("read:%s\n",buff);

  close(fd);
  return 0;
}

