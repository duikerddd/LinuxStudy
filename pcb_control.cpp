#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include <algorithm>  
using namespace std;

int choice;  //选项
int n;       //进程数  

bool ok[100] = {0};    //作业完成情况 1为完成 0为未完成
int last[100] = { 0 };  //任务剩余时间 

struct pcb{
	int pid;   //进程号
	int atime;   //到达时间
	int rtime;   //运行时间
	int stime;   //开始时间
	int etime;   //结束时间
	int time;    //周转时间
	float dtime;   //带权周转时间
	int priority;  //优先级
	int timechip; //时间片
}pro[100];  //最大进程数组,输入输出参数


//先来先服务算法进程排序规则, 先按照到达时间升序，再按照进程号升序排列  
bool fcfscmp(pcb a,pcb  b) {
	if (a.atime != b.atime)return a.atime < b.atime;
	return a.pid < b.pid;
}

//动态优先级算法进程排序规则，先按照进程优先数升序，再按到达时间升序，再按进程号升序排列  
bool pricmp(pcb a, pcb b) {
	if (a.atime != b.atime)return a.atime < b.atime;
	if (a.priority != b.priority)return a.priority > b.priority;
	return a.pid < b.pid;
}

//先来先服务
void FCFS()
{
	int time = 0;
	int t = 0;
	int dt = 0;
	//排序到达时间最早的作业
	sort(pro + 1, pro + 1 + n, fcfscmp);
	//依次完成作业
	printf("\n==== PID 到达时间 运行时间 开始时间 结束时间 周转时间 带权周转时间 ====\n");
	for (int i = 1; i <= n; i++) {   
		
		pro[i].stime = max(time, pro[i].atime);//开始时间 = max(系统时间,到达时间)
		pro[i].etime = pro[i].stime + pro[i].rtime;
		pro[i].time = pro[i].etime - pro[i].atime; //周转时间 = 结束 - 到达
	    pro[i].dtime = pro[i].time/pro[i].rtime; //带权周转时间 = 周转时间/运行时间
		
		time = pro[i].etime;
		//总周转时间,总带权周转时间
		t += pro[i].time;
		dt += pro[i].dtime;
		
		printf("      %d     %d        %d         %d       %d        %d          %3.3f\n",
			pro[i].pid, pro[i].atime, pro[i].rtime, pro[i].stime,pro[i].etime,pro[i].time,pro[i].dtime);
	}
	float avet = (float)t / n;
	float avedt = (float)dt / n;
	printf("平均周转时间为%3.3f,平均带权周转时间为%3.3f\n",avet,avedt);
	
}

//短作业优先
void SPF()
{
	//作业完成情况先置0
	memset(ok, 0, sizeof(ok));
	int t = 0; //周转时间
	int dt = 0;  //带权周转时间
	int time = 0;
	int min = 1;
	printf("\n==== PID 到达时间 运行时间 开始时间 结束时间 周转时间 带权周转时间 ====\n");
	for (int i = 1; i <= n; i++) {

		//1.找出当前最短时间作业
		int flag = 0;  //标记是否有到达作业
		while (flag == 0){
			for (int j = 1; j <= n; j++) {
				if (ok[j] == 1)continue; //已经运行完成的进程  
				if (pro[j].atime > t)continue; //没有到达的进程  
				//出现一个已到达进程
				if (pro[j].atime <= t && flag == 0) {
					min = j;
					flag = 1;
				}
				//出现两个已到达进程,比较出最短时间进程
				else if (pro[j].atime <= t && flag == 1) {
					if ((pro[j].rtime < pro[min].rtime) || (pro[j].rtime == pro[min].rtime&&pro[j].pid < pro[min].pid)) {
						//运行时间短的进程优先                     运行时间相同时,按顺序  
						min = j;
					}
				}
			}// end of for

			//此时,没有进程到达,时间+1
			if (flag == 0) { t++; time++; }
		}

		//2.完成最短作业
		pro[min].stime = max(time, pro[min].atime);
		pro[min].etime = pro[min].stime + pro[min].rtime;
		pro[min].time = pro[min].etime - pro[min].atime; //周转时间 = 结束 - 到达
		pro[min].dtime = pro[min].time / pro[min].rtime; //带权周转时间 = 周转时间/运行时间

		//时间轴 
		time += pro[min].rtime;
		//总周转时间,总带权周转时间
		t += pro[min].time;
		dt += pro[min].dtime;

		printf("      %d     %d        %d         %d       %d        %d          %3.3f\n",
			pro[min].pid, pro[min].atime, pro[min].rtime, pro[min].stime, pro[min].etime, pro[min].time, pro[min].dtime);

		ok[min] = 1;  //标记这个进程完成
	}
	float avet = (float)t / n;
	float avedt = (float)dt / n;
	printf("平均周转时间为%3.3f,平均带权周转时间为%3.3f\n", avet, avedt);

}

/*判断是否全部进程都执行完毕*/
int charge()
{
	for (int i = 1; i<=n; i++)
	{
		if (last[i] != 0)
			return 1;
	}
	return 0;
}
//轮转法
void RR() {
	//作业完成数组置0
	memset(ok, 0, sizeof(ok));
	int t = 0 , dt = 0;
	sort(pro + 1, pro + n + 1, fcfscmp);//排序:先来先服务原则  --- 有利于后面轮询
	
	int i = 0;
	//*关键 : 记录进程剩余时间
	for (int i = 1; i <= n; i++)
	{
		last[i] = pro[i].rtime;
	}
	int chip = pro[1].timechip; //时间片
	int time = pro[1].atime;      //当前时间的初值 
	
	//按顺序轮询,就绪态运行,阻塞态等待到达时间
	printf("\n==== PID 到达时间 运行时间 开始时间 结束时间 周转时间 带权周转时间 ====\n");
	while (charge()) //能进charge说明还有进程
	{
		int flag = 0; //标记是否还有就绪任务
		for (i = 1; i <= n; i++)    //时间片轮转法执行各进程 
		{
			if (ok[i] == 1)
				continue;  //已完成的进程 
				
				//就绪进程轮转
				if (last[i]<=chip && time>=pro[i].atime)//未完成的进程但是还需服务的时间少于等于一个时间片 
				{
					flag = 1;
					//记录第一次开始运行时间
					if (pro[i].rtime == last[i]) 
						pro[i].stime = time; //开始时间 = 当前时间

					//时间更新,任务完成
					time += last[i];
					last[i] = 0;
					ok[i] = 1;
	
					pro[i].etime = time; //结束时间
					pro[i].time = pro[i].etime - pro[i].atime; //周转时间 = 结束 - 到达
					pro[i].dtime = pro[i].time / pro[i].rtime; //带权周转时间 = 周转时间/运行时间

					//总周转时间,总带权周转时间
					t += pro[i].time;
					dt += pro[i].dtime;

					printf("      %d     %d        %d         %d       %d        %d          %3.3f\n",
						pro[i].pid, pro[i].atime, pro[i].rtime, pro[i].stime, pro[i].etime, pro[i].time, pro[i].dtime);
						
				}
				else if (last[i]>chip && time>=pro[i].atime)//未完成的进程但其还需服务时间至少大于一个时间片 
				{
					flag = 1;
					//记录第一次开始运行时间
					if (last[i]==pro[i].rtime)
						pro[i].stime = time; //开始时间 = 当前时间

					time += chip;
					last[i] -= chip;
					
				}
		}//end of for

		//没有一个就绪进程,自增时间
		if (flag == 0)
		{
			time += chip;
		}
	}//end of while
	float avet = (float)t / n;
	float avedt = (float)dt / n;
	printf("平均周转时间为%3.3f,平均带权周转时间为%3.3f\n", avet, avedt);

}

//优先级调度
void PRI() {
	int time = 0;
	int t = 0, dt = 0;
	sort(pro + 1, pro + n + 1, pricmp);//排序:到达时间

	//依次完成作业
	printf("\n==== PID 到达时间 运行时间 开始时间 结束时间 周转时间 带权周转时间 ====\n");
	for (int i = 1; i <= n; i++) {

		pro[i].stime = max(time, pro[i].atime);//开始时间 = max(系统时间,到达时间)
		pro[i].etime = pro[i].stime + pro[i].rtime;
		pro[i].time = pro[i].etime - pro[i].atime; //周转时间 = 结束 - 到达
		pro[i].dtime = pro[i].time / pro[i].rtime; //带权周转时间 = 周转时间/运行时间

		time = pro[i].etime;
		//总周转时间,总带权周转时间
		t += pro[i].time;
		dt += pro[i].dtime;

		printf("      %d     %d        %d         %d       %d        %d          %3.3f\n",
			pro[i].pid, pro[i].atime, pro[i].rtime, pro[i].stime, pro[i].etime, pro[i].time, pro[i].dtime);
	}
	float avet = (float)t / n;
	float avedt = (float)dt / n;
	printf("平均周转时间为%3.3f,平均带权周转时间为%3.3f\n", avet, avedt);

}

//输入界面
void Menu()
{
	while (1){
		n = 0;
		choice = 0;
		printf("=== 请选择算法: ===\n*   1.先来先服务   *\n*   2.短作业优先   *\n*   3.时间片轮转   *\n*   4.优先级调度   *\n*   0.退出         *\n选择:");
		scanf("%d", &choice);
		if (choice == 0) 
			return;
		printf("请选择进程数:");
		scanf("%d", &n);
		printf("***            请依次写入           ***\n PID 到达时间 运行时间 优先级 时间片大小:\n");
		for (int i = 1; i <= n; ++i) {
			scanf("%d %d %d %d %d", &pro[i].pid, &pro[i].atime, &pro[i].rtime, &pro[i].priority, &pro[i].timechip);
		}
		if (n == 0)
			return;
		switch (choice) {
		case 1:FCFS(); break;//先来先服务调度算法  
		case 2:SPF(); break;//短作业优先调度算法  
		case 3:RR(); break;//时间片轮转调度算法  
		case 4:PRI(); break;//优先级调度算法  
		}
		printf("\n");
	}
}

int main()
{
	Menu();
	return 0;
}
