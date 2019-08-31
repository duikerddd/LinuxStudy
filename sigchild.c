// 非阻塞等待回收子进程
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

void sigcb(int signo)
{
  //只有子进程退出的时候,父进程才需要来管,返回值>0表示一直接收,直到没有子进程退出或者出错
  while(waitpid(-1,NULL,WNOHANG)>0);
  printf("give sun money,continue majiang\n");
}

int main()
{
  signal(SIGCHLD,sigcb);
  int pid = fork();
  if(pid==0)
  {
    sleep(10);
    return 0;
  }
  while(1)
  {
    printf("play majiang~\n");
    sleep(1);
  }
  return 0;
}
