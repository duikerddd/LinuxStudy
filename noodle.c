//这是一个吃面卖面的条件变量demo
//pthread_cond_t pthread_cond_init  pthread_cond_wait pthread_cond_signal pthread_cond_broadcast 
//pthread_cond_destory
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<pthread.h>

pthread_cond_t empty;//多个角色需要多个条件变量分别等待,否则会出现唤醒角色错误的问题
pthread_cond_t full;
pthread_mutex_t mutex;
int noodles = 1;

void* thr_sale()
{

  while(1)
  {
    pthread_mutex_lock(&mutex);
    //逻辑:提前做好卖面
    while(noodles != 0)
    {
    pthread_cond_wait(&empty,&mutex);
    } 
    
    //int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex ,
    //const struct timespec *restrict abstime);
    //        int pthread_cond_wait(pthread_cond_t *restrict cond,
    //                         pthread_mutex_t *restrict mutex);
    //等待
    usleep(1000);
    ++noodles;
    printf("seller: have noodles!\n");
    //int pthread_cond_signal(pthread_cond_t *cond);
    //int pthread_cond_broadcast(pthread_cond_t *cond);
    //唤醒 
    pthread_cond_signal(&full);
    pthread_mutex_unlock(&mutex);
  }
}

void* thr_eat()
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    while(noodles != 1)
    {
    pthread_cond_wait(&full,&mutex);
    }

    sleep(1);
    printf("customer: noodles delicious!\n");
    --noodles;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&mutex);
  }
}

int main()
{
  pthread_cond_init(&empty,NULL);
  pthread_cond_init(&full,NULL);
  pthread_mutex_init(&mutex,NULL);
  
  int i;
  pthread_t tid1[2],tid2[4];
  int ret;
  for(int i=0;i<2;++i){
  ret=pthread_create(&tid1[i],NULL,thr_sale,NULL);
  if(ret<0)
  {
    perror("pthread_create error\n");
    return -1;
  }
  }

  for(int i=0;i<4;++i){
  ret=pthread_create(&tid2[i],NULL,thr_eat,NULL);
  if(ret<0)
  {
    perror("pthread_create error\n");
    return -1;
  }
  }
  
  for(int i=0;i<2;++i){
  pthread_join(tid1[i],NULL);
}
  for(int i=0;i<4;++i){
  pthread_join(tid2[i],NULL);
}
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&empty);
  pthread_cond_destroy(&full);

  return 0;
}
