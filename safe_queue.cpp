//基于c++的生产者消费者的安全队列demo
//实现的主要是一个线程安全的队列
//一个场所,两类角色,三种关系
#include<stdio.h>
#include<iostream>
#include<string>
#include<queue>
#include<pthread.h>
#include<unistd.h>
using namespace std;

class SafeQueue
{
    private:
      queue<int> _list; 
      int _capacity;
      pthread_mutex_t _mutex;
      pthread_cond_t _productor;
      pthread_cond_t _consumer;

      bool QueueIsFull()
      {
          return (_list.size()==_capacity?true:false);
      }
      bool QueueIsEmpty()
      {
          return (_list.size()==0?true:false);
      }

    public:
        SafeQueue(int capacity=10):_capacity(capacity)
        {
          pthread_mutex_init (&_mutex,NULL);
          pthread_cond_init(&_consumer,NULL);
          pthread_cond_init(&_productor,NULL);
        }  
        ~SafeQueue()
        {
          pthread_mutex_destroy(&_mutex);
          pthread_cond_destroy(&_consumer);
          pthread_cond_destroy(&_productor);
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
          pthread_cond_wait(&_consumer,&_mutex);
        }
        void CusWakeup()
        {
         pthread_cond_signal(&_productor); 
        }
        void ProWait()
        {
          pthread_cond_wait(&_productor,&_mutex);
        }
        void ProWakeup()
        {
          pthread_cond_signal(&_consumer);
        }
        //放数据---满了等待,否则生产
        bool QueuePush(int data)
        {
            //互斥---对队列数据(临界资源)的唯一访问性
            QueueLock();

            while(QueueIsFull())
            {
              printf("queue is full!\n");
              //同步---对队列数据(临界资源)的时序可控性
              ProWait();
            }
            _list.push(data);

            QueueUnlock();
            ProWakeup();
            return true;
        }
        //取数据---空了等待,否则取出
        bool QueuePop(int *data)
        {
            QueueLock();

            while(QueueIsEmpty())
            {
              printf("queue is empty\n");
              CusWait();
            }
            *data = _list.front();
            _list.pop();

            QueueUnlock();
            CusWakeup();
            return true;
        }

};

//消费者从队列中取数据
void* thr_consumer(void* arg)
{
  SafeQueue *q = (SafeQueue*)arg;
  while(1)
  {
    int data;
    if(q->QueuePop(&data))
    {
      usleep(1000);
      printf("consumer get data:[%d]\n",data);
    }
  }
  return NULL;
}

//生产者在队列中生产
void* thr_productor(void* arg)
{
  SafeQueue* q = (SafeQueue*)arg;
  int i = 0;
  while(1)
  {
    printf("put data --- %d\n",i);
    q->QueuePush(i++);
    usleep(10000);
  }
  return NULL;
}

int main()
{
  //测试
  SafeQueue safe_queue;
  pthread_t ctid[4],ptid[4];
  for(int i=0;i<4;++i)
  {
    int ret=pthread_create(&ctid[i],NULL,thr_consumer,(void*)&safe_queue);
    if(ret!=0)
    {
      printf("creat error\n");
      return -1;
    }
    ret=pthread_create(&ptid[i],NULL,thr_productor,(void*)&safe_queue);
    if(ret!=0)
    {
      printf("creat error\n");
      return -1;
    } 
  }

  for(int i=0;i<4;i++)
  {
    pthread_join(ctid[i],NULL);
    pthread_join(ptid[i],NULL);      
  }
  return 0;
}
