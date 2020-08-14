#include <stdio.h>

//  宏函数,某些情况下比函数效率高,因为宏函数只是进行简单的文本替换,不进行任何的语法检查
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


