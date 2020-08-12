#include <stdio.h>

extern int* getSpace();
extern void allocateSpace(char*);
extern void allocateSpace01(char**);

int main()
{
//    ����p�ĵ�ַ��һ����,�����ڵ�������ͷ���.Ϊɶ��һ���ĵ�ַ��??
    int* p1=getSpace();
    printf("%p\n",p1);


//    ������ڴ�һ��Ҫ�ͷ�
    free(p1);
    p1=NULL;


    char* p=NULL;
    allocateSpace(p);
    printf("p=%s\n",p);//p=null

    char* p01=NULL;
    allocateSpace01(&p01);
    printf("p=%s\n",p01);//p=hello world
}

//�β�Ҳ�Ǿֲ�����,֮���ͷŵ���.�������еĶ��ڴ涪ʧ�˵�ַ,"hello world"���ɳ��������Ƶ������е�
void allocateSpace(char* p)
{
    p=malloc(100);
    memset(p,0,100);
    strcpy(p,"hello world!");
}

//�������ָ��,�Ƕ���һ���ĸĽ�
void allocateSpace01(char** p)
{
    char* temp=malloc(100);
    memset(temp,0,100);
    strcpy(temp,"hello world!");

    *p=temp;
}

int* getSpace()
{
//    p������ջ��,ָ���˶������ڴ�
    int* p=malloc(sizeof(int)*5);
//    ���ڴ�����֮��һ��Ҫ���г�ʼ��,�ܶ�bug����û�г�ʼ����ɵ�
    memset(p,0,20);
    printf("%p\n",p);
    if(NULL==p)
    {
        return NULL;
    }

//    ֻҪ���ڴ���������,������ʹ���±���ʹ��
    for(int i=0; i<5; ++i)
    {
        p[i]=100+i;
    }

    return p;

}
