//C语言io接口
//fopen(r r+ w w+ a a+)
//fclose fwrite fread fseek跳转 ftell获得文件偏移位置
//fprintf  snprintf sprintf printf
#include<stdio.h>
#include<string.h>

int main()
{
	FILE* fd = fopen("./test.txt","w+");

	char *ptr = "it's a text";
	fwrite(ptr,strlen(ptr),1,fd);

	//fseek(FILE* stream,long offset,int whence);
	//offset :  偏移大小
	//whence :  偏移位置
	//	SEEK_SET 文件起始位置
	//	SEEK_CUR 当前读写位置
	//	SEEK_END 文件末尾位置	
	fseek(fd,0,SEEK_SET);	

	char buff[1024] = {0};
	fread(buff,1023,1,fd);
	printf("read success:%s\n",buff);	

	fclose(fd);
	return 0;
}
