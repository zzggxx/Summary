#include <stdio.h>

//  �꺯��,ĳЩ����±Ⱥ���Ч�ʸ�,��Ϊ�꺯��ֻ�ǽ��м򵥵��ı��滻,�������κε��﷨���
#define MYADD(x,y) ((x)+(y))
#define MAX 1024

extern void testtt(char**,int*);

int main()
{

int arr[]={1,2,3,4};

printf("%d\n",arr);
printf("%d\n",&arr);
printf("%d\n",&arr+1);

printf("%d\n",arr[-1]);

}


