#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<sys/types.h>
#include <sys/stat.h>
//10M大小
#define MAXSIZE 1024*1024*10


int main05()
{
	unsigned int start_time= time(NULL);

	//mycp    wow.2.mp4 wow.3.mp4
	//argv[0] argv[1]   arr[2]
	FILE * fp1 = fopen("E://c_project//update.zip","r");
	FILE * fp2 = fopen("E://c_project//update1.zip","w");
	if(!fp1 || !fp2)
	{
		printf("操作文件失败\n");
		return -2;
	}

	//获取文件属性
	struct stat *s = NULL;
	//获取源文件
	stat("E://c_project//update.zip",s);
	char * ch;
	int maxSize=0;
	if(s->st_size < MAXSIZE)
	{
		maxSize =s->st_size;
		ch = (char *)malloc(sizeof(char)*s->st_size);
	}
	else
	{
		maxSize = MAXSIZE;
		ch = (char *)malloc(sizeof(char)*MAXSIZE);
	}
	while(!feof(fp1))
	{
		//memset(ch,0,maxSize);
		int len = fread(ch,1,100,fp1);//123
		fwrite(ch,1,len,fp2);
	}


	fclose(fp1);
	fclose(fp2);

	free(ch);
	unsigned int end_time=time(NULL);

	printf("花费时间：%d(s)\n",end_time-start_time);

	return 0;

}

