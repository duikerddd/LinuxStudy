#include<stdio.h>
#include <iostream>
#include<iomanip>
using namespace std;
#define MAXPROCESS 1000 /*最大进程数*/ 
#define MAXRESOURCE 1000 /*最大资源数*/ 

int AVAILABLE[MAXRESOURCE]; /*可用资源数组*/
int MAX[MAXPROCESS][MAXRESOURCE]; /*最大需求矩阵*/
int ALLOCATION[MAXPROCESS][MAXRESOURCE]; /*分配矩阵*/
int NEED[MAXPROCESS][MAXRESOURCE]; /*需求矩阵*/
int REQUEST[MAXPROCESS][MAXRESOURCE]; /*进程需要资源数*/
bool FINISH[MAXPROCESS]; /*系统是否有足够的资源分配*/
int p[MAXPROCESS]; /*记录序列*/
int m, n; /*m个进程,n个资源*/
int bank_flag = 0;/*标志是否有初始化资源*/

void Init();
bool Safe();
void Bank();
void Menu();
void Check();
void Show();

//主函数
int main()
{
	Menu();
}

//主界面
void Menu()
{
	while (1){
		printf("=========================\n");
		printf("*      银行家算法       *\n");
		printf("*     1.设置资源初值    *\n");
		printf("*     2.安全性检测      *\n");
		printf("*     3.申请资源        *\n");
		printf("*     4.显示资源        *\n");
		printf("*     0.退出            *\n");
		printf("=========================\n");
		printf("请输入您的选项:");
		int choice = 0;
		cin >> choice;
		switch (choice)
		{
		case 1:
			Init();
			break;
		case 2:
			Safe();
			break;
		case 3:
			Bank();
			break;
		case 4:
			Show();
			break;
		case 0:
			return;
		default:
			cout << "输入非法,请重新输入!" << endl;
		}
	}
}

//展示资源
void Show()
{
	if (bank_flag == 0)
	{
		cout << "请设置资源初值!" << endl;
		cout << endl;
		return;
	}
	//设置左对齐
	cout << setiosflags(ios::left);
	int w = 18;
	cout << setw(w) << "Pid";
	cout << setw(w) << "Max";
	cout << setw(w) << "Allocation";
	cout << setw(w) << "Need";
	cout << setw(w) << "Available" << endl;

	for (int i = 0; i < m; i++)
	{
		cout << setw(15) << i;
		int wid = 18 / n;
		for (int j = 0; j < n; j++)
		{
			cout << setw(wid) << MAX[i][j];
		}
		cout << " ";
		for (int j = 0; j < n; j++)
		{
			cout << setw(wid) << ALLOCATION[i][j];
		}
		cout << " ";
		for (int j = 0; j < n; j++)
		{
			cout << setw(wid) << NEED[i][j];
		}
		cout << " ";
		if (i == 0)
		{
			for (int z = 0; z < n; z++){
				cout << setw(wid) << AVAILABLE[z];
			}
		}
		cout << endl;
	}
	cout << endl;
}

//回退步骤
bool Check(int flag)
{
	return flag ==  -1? true : false;
}
//初始化
void Init() 
{
	cout << "如需回到主界面,输入-1" << endl;
	int i, j;
	cout << "请输入进程的数目:";
	cin >> m;
	if (Check(m))
		return;

	cout << "请输入资源的种类:";
	cin >> n;
	if (Check(n))
		return;

	cout << "请输入每个进程最多所需的各资源数,按照" << m << "x" << n << "矩阵输入" << endl;
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			cin >> MAX[i][j];
			if (Check(MAX[i][j]))
				return;
		}
	}

	cout << "请输入每个进程已分配的各资源数,也按照" << m << "x" << n << "矩阵输入" << endl;
	for (i = 0; i<m; i++)
	{
		for (j = 0; j<n; j++)
		{
			cin >> ALLOCATION[i][j];
			NEED[i][j] = MAX[i][j] - ALLOCATION[i][j];

			if (Check(ALLOCATION[i][j]))
				return;
			if (NEED[i][j]<0)
			{
				cout << "您输入的第" << i + 1 << "个进程所拥有的第" << j + 1 << "个资源数错误,请重新输入:" << endl;
				j--;
				continue;
			}
		}
	}
	cout << "请输入各个资源现有的数目:" << endl;
	for (i = 0; i<n; i++)
	{
		cin >> AVAILABLE[i];
		if (Check(AVAILABLE[i]))
			return;
	}
	bank_flag = 1; //资源初始化成功标志
	cout << endl;
}

//银行家算法
void Bank() 
{
		if (bank_flag == 0)
		{
			cout << "请设置资源初值!" << endl;
			cout << endl;
			return;
		}
		int i, cusneed;

		int flag = 0;//银行家算法4步骤成功标志
		
		//请求资源初始化
		cout << "请输入要申请资源的进程号(下标从0开始):";
		cin >> cusneed;
		cout << "请输入进程所请求的各资源的数量:";
		for (i = 0; i<n; i++)
		{
			cin >> REQUEST[cusneed][i];
		}

		//1.request[i][j]<=need[i][j]
		//2.request[i][j]<=aval[i][j]
		for (i = 0; i<n; i++)
		{
			if (REQUEST[cusneed][i]>NEED[cusneed][i])
			{
				cout << "请求数超过进程的需求量!" << endl;
				flag = 1;
				break;
			}
			if (REQUEST[cusneed][i]>AVAILABLE[i])
			{
				cout << "请求数超过系统有的资源数!" << endl;
				flag = 1;
				break;
			}
		}
		if (flag == 1)
			return;

		//3.试图分配
		//现有资源-req;i分配资源+req;i需求-req;
		for (i = 0; i<n; i++)
		{
			AVAILABLE[i] -= REQUEST[cusneed][i];
			ALLOCATION[cusneed][i] += REQUEST[cusneed][i];
			NEED[cusneed][i] -= REQUEST[cusneed][i];
		}
		//4.安全性算法
		if (Safe())
		{
			cout << "同意分配请求!" << endl;
		}
		else
		{
			//请求拒绝,还原资源
			cout << "请求被拒绝!" << endl;
			for (i = 0; i<n; i++)
			{
				AVAILABLE[i] += REQUEST[cusneed][i];
				ALLOCATION[cusneed][i] -= REQUEST[cusneed][i];
				NEED[cusneed][i] += REQUEST[cusneed][i];
			}
		}
		//将完成标志重置为false
		for (i = 0; i<m; i++)
		{
			FINISH[i] = false;
		}
	
		cout << endl;
}

//安全性算法
bool Safe() 
{
	if (bank_flag == 0)
	{
		cout << "请设置资源初值!" << endl;
		cout << endl;
		return false;
	}

	int i, j, k, l = 0;
	int Work[MAXRESOURCE]; //工作数组

	for (i = 0; i<n; i++)  //初始化work
		Work[i] = AVAILABLE[i];
	for (i = 0; i<m; i++)
	{
		FINISH[i] = false;
	}

	for (i = 0; i<m; i++)
	{
		//进程资源已回收,找下一个
		if (FINISH[i] == true)
		{
			continue;
		}
		//进程资源未会回收
		else
		{
			//work[j]<need[i][j],不满足
			for (j = 0; j<n; j++)
			{
				if (NEED[i][j]>Work[j])
				{
					break;
				}
			}
			//work[j]>need[i][j],回收此进程资源
			if (j == n)
			{
				FINISH[i] = true;
				for (k = 0; k<n; k++)
				{
					Work[k] += ALLOCATION[i][k];
				}
				//回收的进程放入记录序列
				p[l++] = i;
				//重头继续找
				i = -1;
			}
			else
			{
				continue;
			}
		}
		//如果进程全部完成,则返回安全对列
		if (l == m)
		{
			cout << "系统是安全的" << endl;
			cout << "安全序列:" ;
			for (i = 0; i<l; i++)
			{
				cout << p[i];
				if (i != l - 1)
				{
					cout << "-->";
				}
			}
			cout << "" << endl;
			cout << endl;
			return true;
		}
	}//end of for

	//进程有未完成的,不安全
	cout << "系统是不安全的" << endl;
	cout << endl;
	return false;
}
