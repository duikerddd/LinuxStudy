//共享内存demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#define IPC_KEY 0x123456
#define SHM_SIZE 4096

int main()
{
	int shmid = shmget(IPC_KEY,SHM_SIZE,IPC_CREAT | 0664);
	void* shm_start = (char*)shmat(shmid,NULL,0);	
	while(1)
	{
		printf("%s\n",shm_start);
		sleep(1);
	}
	shmdt(shm_start);
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}
