//可重入不可重入demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <signal.h>


int a=0;
int b=0;

//test执行流
void test()
{
  a++;
  sleep(3);
  b++;
}

//打断信号执行流
void sigcb(int signo)
{
  test();
  printf("SIGINT : a+b = %d\n",a+b);
}

int main()
{
  signal(SIGINT,sigcb);
  test();
  printf("test : a+b = %d\n",a+b);
  return 0;
}
