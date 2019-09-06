//黄牛抢票(互斥锁)demo
//   pthread_mutex pthread_mutex_lock pthread_mutex_unlock pthread_mutex_init pthread_mutex_destroy
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<string.h>

volatile int ticket = 100;
pthread_mutex_t mutex;

void* thr_start(void* arg)
{
  int id = (int)arg;
  while(1)
  {
    pthread_mutex_lock(&mutex);
    if(ticket>0)
    {
      usleep(1000);
       printf("%d yellow cattle get no.%d ticket\n",id,ticket);
       --ticket;
    }
    else{
      //在线程可能退出的地方都要解锁
      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    }
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main()
{
  pthread_t tid[4];
  int i,ret;
  // 互斥锁创建
  // int pthread_mutex_init(pthread_mutex_t *restrict mutex,
  // const pthread_mutexattr_t *restrict attr);
  // mutex:互斥锁   attr:属性
  // pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_init(&mutex,NULL);              

  for(i=0;i<4;++i)
  {
    ret = pthread_create(&tid[i],NULL,thr_start,(void*)i);
    if(ret<0)
    {
      perror("pthread_create error\n");
      return -1;
    }
  }
  for(i=0;i<4;++i)
  {
    pthread_join(tid[i],NULL);
  }
  //互斥锁销毁
  //int pthread_mutex_destroy(pthread_mutex_t *mutex);
  pthread_mutex_destroy(&mutex);
  return 0;
}
