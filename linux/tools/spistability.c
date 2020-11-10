#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MIN_FILE_CHAR_SIZE 1
#define MAX_FILE_CHAR_SIZE 4096

#define MIN_CHAR_START 48
#define MAX_CHAR_END 126

int my_rand(int min,int max);
void getdata(char *data,int randnum);
char getthecontrol();

void main(void)
{
	//get rand so many char
	srand((unsigned int)(time(NULL)));
	int randnum = my_rand(MIN_FILE_CHAR_SIZE,MAX_FILE_CHAR_SIZE);
	char data[randnum];
	getdata(data,randnum);
	printf("the data is:%s\n",data);
	
	//copy the data to file
	FILE* fp=fopen("/result.txt","w");
	if(fp<0)
	{
		printf("open the file is wrong\n");
		return;
	}
	int i=0;
	while(data[i] != '\0')
	{
		fprintf(fp,"%c",data[i]);
		i++;
	}
	fclose(fp);

// get GPIO control
	while(1){
		char aa = getthecontrol();
		printf("%c\n",aa);
		
// send data throu
		if(0 == aa)
		{
			spisenddata();
			return;
		}
	}
}

char getthecontrol()
{
	FILE* fp = fopen("/sys/class/gpio/gpio55/value","r");
	if(fp<0){
		printf("open the gpio55/value is wrong\n");
		return -1;
	}
	char ch;
	while((ch=fgetc(fp)) != EOF)
	{
		return ch;
	}
	return '\0';
}

void getdata(char *data,int randnum)
{
	memset(data,0,randnum);
	printf("the char num is %d\n",randnum);
	for(int i=0;i<randnum;i++)
	{
		int a = my_rand(MIN_CHAR_START,MAX_CHAR_END);
		char x = (char)a;
		//printf("%d,%d,%c;",i,a,x);
		*(data+i) = (char)a;
	}
}

// get rand num.bettwen min and max
int my_rand(int min,int max)
{
	int num;
	num = rand();
	num = num % max;
	while(num < min){
		num = (num+min) % max;
	}
	return num;
}
