//c++实现线程池 --- 一个或多个 线程安全  任务 队列
//功能 : 1.避免峰值压力,导致资源耗尽
//       2.避免大量的创建/销毁线程成本
//       std:queue<task> _q    _cap
//       _thr_max _thr_cur
//       _mutex  _full  _empty   
//       QueueIsFull QueueIsEmpty
//       QueueLock  QueueUnlock
//       QueuePush  QueuePop
//       thr_start
//       Init   AddTask
//       class Task
#include<stdio.h>
#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

using namespace std;

//函数指针类型
typedef bool (*taskCallback)(int data);
//任务接口---创建/销毁/设置任务,启动任务
class MyTask
{
  private:
    int _data;
    taskCallback _callback;
  public:
    void SetTask(int data,taskCallback callback)
    {
      _data = data;
      _callback=callback;
    }
    /*
    void Run()
    {
      srand(time(NULL));
      int s = rand() % 5;
      printf("thread%p is run,get data %d,sleep -- %d\n",pthread_self(),_data,s);
      sleep(s);
    }
    */
    bool Run()
    {
      //执行回调函数---函数指针可以这样使用
      return _callback(_data);
    }
};


//线程池管理---创建/销毁任务,添加任务
class ThreadPool
{
  private:
    //任务队列
    queue<MyTask> _queue;
    int _cap;
    int _thr_max;
    int _thr_cur;
    pthread_mutex_t _mutex;
    //有货
    pthread_cond_t _full;
    //缺货
    pthread_cond_t _empty;
    //线程池线程的退出标志
    bool _quit_flag;
    bool QueueIsFull()
    {
      return (_queue.size()==_cap?true:false);
    }
    bool QueueIsEmpty()
    {
      return (_queue.size()==0?true:false);
    }
    void  QueuePush(MyTask &t)
    {
      _queue.push(t);
    }
    void QueuePop(MyTask *t)
    {
      *t = _queue.front();
      _queue.pop();
    }
    //工作线程---相当于消费者,循环进行
    static void* thr_start(void* arg)
    {
      ThreadPool *q = (ThreadPool*)arg;
      
      while(1)
      {
        q->QueueLock();

        //没有等待
        while(q->QueueIsEmpty())
        {
          q->RunWait();
        }
        //有了处理
        MyTask task;
        q->QueuePop(&task);
        q->AddWakeup();

        q->QueueUnlock();
        //发送信号了,再解决任务,提高效率
        task.Run();
      }
      
      return NULL;
    }

  public:
    ThreadPool(int max=5,int cap=10):_thr_max(max)
                                     ,_cap(cap)
    {
    _quit_flag=false; 
    pthread_mutex_init(&_mutex,NULL);
    pthread_cond_init(&_full,NULL);
    pthread_cond_init(&_empty,NULL);
    }
    ~ThreadPool()
    {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_full);
    pthread_cond_destroy(&_empty);
    }
    void QueueLock()
    {
      pthread_mutex_lock(&_mutex);
    }
    void QueueUnlock()
    {
      pthread_mutex_unlock(&_mutex);
    }
    void AddWait()
    {
      pthread_cond_wait(&_empty,&_mutex);
    }
    void RunWakeup()
    {
      pthread_cond_signal(&_full);
    }
    void RunAllWakeup()
    {
      pthread_cond_signal(&_full);
    }
    void RunWait()
    {
      //若用户想要线程退出,则需要选择没有任务的时候退出
      if(_quit_flag==true)
      {
        _thr_cur--;
        pthread_mutex_unlock(&_mutex);
        printf("thread %p exit\n",pthread_self()); 
        pthread_exit(NULL);
      }
      pthread_cond_wait(&_full,&_mutex);
    }
    void AddWakeup()
    {
      pthread_cond_signal(&_empty);
    } 
    //创建线程
    bool Init()
    {
      pthread_t tid;
      for(int i=0;i<_thr_max;++i)
      {
        //这里传this指针,是因为静态函数里面没有this指针
        int ret=pthread_create(&tid,NULL,thr_start,(void*)this); 
        if(ret!=0)
        {
          printf("create error");
          return false;
        }
        pthread_detach(tid);
      }
      return true;
    }
    //添加任务---相当于生产者
    bool AddTask(MyTask &task)
    {
          QueueLock();
  
          //如果一直加任务,队列不为空,就不会走到runwait退出
          if(_quit_flag==true)
          {
            QueueUnlock();
            return false;
          }

          //满了等待
          while(QueueIsFull())
          {
            AddWait();
          }
          //有了插入
          QueuePush(task);
          RunWakeup();

          QueueUnlock();
          return true;
    }
    //线程退出
    void ThreadQuit()
    {
      QueueLock();
      _quit_flag=true;
      QueueUnlock();
      while(_thr_cur>0)
      {
        RunAllWakeup();
        usleep(1000);
      }
    }

};

bool TaskHandler(int data)
{
      srand(time(NULL));
      int s = rand() % 5;
      printf("thread %p is run,get data %d,sleep -- %d\n",pthread_self(),data,s);
      sleep(s);
      return true;
}

int main()
{
  ThreadPool pool;
  pool.Init();
  MyTask task[10];
  for(int i=0;i<10;++i)
  {
    task[i].SetTask(i,TaskHandler);
    pool.AddTask(task[i]);
  }
  pool.ThreadQuit();
  while(1)
  {
  sleep(1);
  }
  return 0;
}


