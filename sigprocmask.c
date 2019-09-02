//信号阻塞demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void sigcb(int signo)
{
	printf("recv singo:%d\n",signo);
}

int main()
{
	struct sigaction act;
	struct sigaction oldact;
	act.sa_flags=0;
	act.sa_handler=sigcb;
	sigemptyset(&act.sa_mask);
        sigaction(SIGINT,&act,NULL);
	sigaction(40,&act,NULL);

	sigset_t newset,oldset;
	//int sigfillset(sigset_t *set);
	//向集合添加所有信号
	//int sigaddset(sigset_t *set,int signum);
	//向集合添加一个指定信号
	//int sigdelset(sigset_t *set,int signum);
	//从集合中移除一个指定信号
	sigemptyset(&oldset);
	sigfillset(&newset);
	
	//int sigprocmask(int how,const sigset_t *set,sigset_t *oldset);
	//how :
	//	SIG_BLOCK    阻塞set信号,将原有阻塞放入old
	//	SIG_UNBLOCK	 解除set阻塞
	//	SIG_SETMASK	 set信号集合添加到阻塞集合
	sigprocmask(SIG_BLOCK,&newset,&oldset);
	printf("all signal blocked ,enter for unblocked\n");
	raise(2);
	//按回车之前信号一直阻塞
	getchar();

	//int sigpending(sigset_t *set);
	//获取未决信号
	sigset_t pending;
	sigpending(&pending);
	int i = 0;
	for(i=1;i<32;++i)
	{
		if(sigismember(&pending,i))
		{
			printf("1 ");
		}
		else
		{
			printf("0 ");
		}
	}
	fflush(stdout);

	//按回车之后,信号解除阻塞
	sigprocmask(SIG_UNBLOCK,&newset,NULL);
	while(1)
	{
		printf("---------\n");
		sleep(1);
	}
	
	return 0;
}
