//sigaction函数demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signo)
{
  printf("i am new signal :) ---%d \n",signo);
}

int main()
{
  struct sigaction act;
  struct sigaction oldact;
  //清空指定集合
  //int sigemptyset(sigset_t *set);
  sigemptyset(&act.sa_mask);

  act.sa_handler = sigcb;
  act.sa_flags = 0;

  //int sigaction(int signum,const struct sigaction *act,struct sigaction *oldact);
  //为信号定义新行为,并将原有行为保存到oldact中
  sigaction(SIGINT,&act,&oldact);

  
  raise(SIGINT);
  while(1)
  {
    printf("-------\n");
    sleep(1);
  }
  
  return 0;
}
