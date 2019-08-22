//获取环境变量demo
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

//1.通过主函数获取环境变量
void env1(char* env[])
{
	int i;
	for(i;env[i]!=NULL;++i)
	{
		printf("env[%d]-%s\n",i,env[i]);
	}
}

//2.通过全局变量extern char* env[]
void env2()
{
	extern char** environ;
	int i;	
	for(i;environ[i]!=NULL;++i)
	{
		printf("env[%d]-%s\n",i,environ[i]);
	}
}

//3.调用getenv()
void env3()
{
	//char *getenv(const char* name);
	//通过获取环境变量的名字,获取到环境变量的内容
	printf("path-[%s]\n",getenv("PATH"));	
	printf("home-[%s]\n",getenv("HOME"));	
}	
int main(int argc,char *argv[],char *env[])
{
	//env1(env);
	//env2();
	env3();
	return 0;
}
