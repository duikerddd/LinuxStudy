//使用信号量实现线程安全队列demo---前后指针
#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<vector>
using namespace std;

class SafeQueue
{
  private:
    vector<int> _list;
    int _cap;
    //前后指针
    int _pro_step;
    int _cus_step;
    //同步 : 资源计数+唤醒等待的信号量
    //_data+_space=_cap 
    sem_t _data;
    sem_t _space;
    //互斥 : 互斥锁,资源只有0/1的时候,信号量才能实现互斥
    pthread_mutex_t _mutex;


  public:
    SafeQueue(int cap=10):_cap(cap)
                          ,_list(cap)
                          ,_pro_step(0)
                          ,_cus_step(0)
   {
          pthread_mutex_init(&_mutex,NULL);
          sem_init(&_data,0,0);
          sem_init(&_space,0,_cap);
   }
   ~SafeQueue()
   {
          pthread_mutex_destroy(&_mutex);
          sem_destroy(&_data);
          sem_destroy(&_space);
   }
   void QueueLock()
   {
     pthread_mutex_lock(&_mutex); 
   }
   void QueueUnlock()
   {
     pthread_mutex_unlock(&_mutex);
   }
   void CusWait()
   {
     sem_wait(&_data);
   }
   void ProWait()
   {
     sem_wait(&_space);
   }
   void CusWakeup()
   {
     sem_post(&_space);
   }
   void ProWakeup()
   {
     sem_post(&_data);
   }
   bool QueuePush(int data)
   {
     QueueLock();

     //判断是否还有空间,有则空间-1,无则等待
     sem_wait(&_space);
     _list[_pro_step]=data;
     _pro_step++;
     //循环队列中指针的移动规律
     _pro_step%=_cap;
     //唤醒消费者,资源+1
     sem_post(&_data);

     QueueUnlock();
     return true;
   }
   bool QueuePop(int *data)
   {
    QueueLock();

    //判断是否还有数据,有则数据-1,无则等待
    sem_wait(&_data);
    *data=_list[_cus_step];
    _cus_step++;
    _cus_step%=_cap;
    //唤醒生产者,空间+1
    sem_post(&_space);

    QueueUnlock();
    return true;
   }

};

void* thr_cus(void* arg)
{
  SafeQueue* sq=(SafeQueue*)arg;
  int data;
  while(1)
  {
    sleep(1);
    sq->QueuePop(&data);
    printf("get data:%d\n",data);
  }
  return NULL;
}

void* thr_pro(void* arg)
{
  SafeQueue* sq=(SafeQueue*)arg;
  int i=0;
  while(1)
  {
    sleep(1);
    sq->QueuePush(i++);
    printf("create a data!\n");
  }
  return NULL;
}

int main()
{
  SafeQueue sq;
  pthread_t ctid[4],ptid[4];
  for(int i=0;i<4;++i)
  {
    int ret=pthread_create(&ctid[i],NULL,thr_cus,(void*)&sq);
    if(ret!=0)
    {
      printf("create error");
      return -1;
    }
    ret=pthread_create(&ptid[i],NULL,thr_pro,(void*)&sq);
    if(ret!=0)
    {
      printf("create error");
      return -1;
    }
  }
  for(int i=0;i<4;++i)
  {
    pthread_join(ctid[i],NULL);
    pthread_join(ptid[i],NULL);
  }
  return 0;
}


