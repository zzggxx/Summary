#include <stdio.h>

extern int* getSpace();
extern void allocateSpace(char*);
extern void allocateSpace01(char**);

int main()
{
//    两个p的地址是一样的,函数内的用完就释放了.为啥是一样的地址呢??
    int* p1=getSpace();
    printf("%p\n",p1);


//    用完堆内存一定要释放
    free(p1);
    p1=NULL;


    char* p=NULL;
    allocateSpace(p);
    printf("p=%s\n",p);//p=null

    char* p01=NULL;
    allocateSpace01(&p01);
    printf("p=%s\n",p01);//p=hello world
}

//形参也是局部变量,之后被释放掉了.并且其中的堆内存丢失了地址,"hello world"是由常量区复制到堆区中的
void allocateSpace(char* p)
{
    p=malloc(100);
    memset(p,0,100);
    strcpy(p,"hello world!");
}

//传入二级指针,是对上一个的改进
void allocateSpace01(char** p)
{
    char* temp=malloc(100);
    memset(temp,0,100);
    strcpy(temp,"hello world!");

    *p=temp;
}

int* getSpace()
{
//    p还是在栈区,指向了堆区的内存
    int* p=malloc(sizeof(int)*5);
//    堆内存申请之后一定要进行初始化,很多bug就是没有初始化造成的
    memset(p,0,20);
    printf("%p\n",p);
    if(NULL==p)
    {
        return NULL;
    }

//    只要是内存是连续的,都可以使用下标来使用
    for(int i=0; i<5; ++i)
    {
        p[i]=100+i;
    }

    return p;

}
