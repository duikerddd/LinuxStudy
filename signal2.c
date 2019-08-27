//修改信号的处理方式demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb()
{
  printf("SIGHUP is change:) \n");
}

int main()
{
  //sighandler_t signal(int signum,sighandler_t hander);
  //handler :   SIG_IGN 忽略 SIG_DFL 默认 func 函数
  signal(SIGHUP,SIG_IGN);
  signal(SIGHUP,sigcb);
  raise(SIGHUP);

  return 0;
}
