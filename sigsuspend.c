//sigsuspend demo
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

void sigcb(int signo)
{
	printf("atomic operation :) \n");
}

int my_sleep(int nsec)
{
  //替换SIGALRM--->防止SIGALRM打断程序
	struct sigaction act,old;
	act.sa_handler = sigcb;
	act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
	sigaction(SIGALRM,&act,&old);
	
  //阻塞SIGALRM,防止被其他信号在返回用户态的时候处理
  sigprocmask(SIG_BLOCK,&act.sa_mask,NULL);

	//mask阻塞集合
	sigset_t set;
	sigfillset(&set);
	sigdelset(&set,SIGALRM);

	alarm(nsec);
	//int sigsuspend(const sigset_t *mask);
	//临时阻塞mask-->替换阻塞集合-->休眠-->被唤醒后,将阻塞集合还原回去
	sigsuspend(&set);
	//pause();
	return 0;
}

int main()
{
	my_sleep(3);
	while(1)
	{
		printf("-------\n");
		sleep(1);
	}
	return 0;
}
