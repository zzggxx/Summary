#include <stdio.h>

//  �꺯��,ĳЩ����±Ⱥ���Ч�ʸ�,��Ϊ�꺯��ֻ�ǽ��м򵥵��ı��滻,�������κε��﷨���
#define MYADD(x,y) ((x)+(y))
#define MAX 1024

extern void testtt(char**,int*);

int main()
{

//    int a[]={1,2,3};
//    printf("%d\n",a);
//
    char arr[]="hello world!";

    reverse(arr,sizeof(arr)/sizeof(arr[0]));

    printf("1111111111\n");
    printf("%d---\n",arr);
}

void reverse(char* arr,int len)
{
//    char arr[]="hello world!";

    char* start=arr;
    char* end=arr+len-1;
    while(start<end)
    {
        char temp=*start;
        *start=*end;
        *end=temp;
        ++start;
        --end;
    }

    printf("%d---\n",arr);
}

