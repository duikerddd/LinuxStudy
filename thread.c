//线程基本使用demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


void* thr_start(void *arg)
{
  printf("thread say: %s\n",arg);
  pthread_t tid = pthread_self();
  while(1)
  {
    printf("i am common thread,%p \n",tid);
    sleep(1);
    //线程退出
    //void pthread_exit(void *retval);
    // pthread_exit(NULL);
  }
  return NULL;
}

int main()
{
 //线程创建
 //int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
 //                     void *(*start_routine) (void *), void *arg);
 //         thread : 输出型参数,获取线程id
 //         attr : 属性,默认null
 //         start_routine : 入口函数
 //         arg : 入口函数传参
 //         返回值:成功0 错误错误码
  pthread_t thread;
  char* str = "hello thread";
  int ret = pthread_create(&thread,NULL,&thr_start,(void*)str);
  if(ret != 0)
  {
    printf("pthread_create error\n");
    return -1;
  }

   printf("pthread_create success\n");

   //指定线程退出,不能指定自己
   //int pthread_cancel(pthread_t thread);
   pthread_cancel(thread);
   while(1)
   {
     printf("i am main thread %p\n",thread);
     sleep(1);
   }
  return 0;
}
