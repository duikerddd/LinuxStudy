//信号软件产生的几种方式demo
//kill ; raise ; abort ; alarm ; sigqueue
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

int main()
{
  //n秒之后,发送SIGALRM信号
  alarm(3);
  sleep(1);
  int ret = alarm(5);
  printf("ret:%d\n",ret);
  
  //向自己指定发送信号
  raise(SIGHUP);

  //向指定进程发送信号
  kill(getpid(),11);
  
  //向自己发送SIGABRT信号
  abort();

  //向指定进程发送信号,并且可以携带一个参数
  sigqueue(getpid(),SIGKILL,(union sigval)999);
  return 0;
}

