//线程等待demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

void* thr_start(void* arg)
{
  printf("i am child\n");
  sleep(3);
  return "i am exit :)";
}

int main()
{
  pthread_t tid;
  int ret = pthread_create(&tid,NULL,&thr_start,NULL);
  if(ret!=0)
  {
    printf("pthread_create error");
    return -1;
  }
  printf("pthread_create success\n");

  //等待线程退出
  //int pthread_join(pthread_t thread, void **retval);
  char* str ;
  pthread_join(tid,(void**)&str);
  printf("child thread say:%s\n",str);
  return 0;
}
