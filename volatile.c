//volatile demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

volatile int i = 1;

void sigcb(int signo)
{
  printf("-------\n");
  i = 0;
}

int main()
{
  signal(SIGINT,sigcb);
  while(i)
  {
    printf("i am bug for optimize\n");
  }
  return 0;
}
