#include <stdio.h>

struct Persion
{
    char* name;
    int age;
};

void print(struct Persion p)
{
    printf("%s %d\n",p.name,p.age);
}

int main()
{
    struct Persion pp1;
    pp1.name = malloc(sizeof(char)*20);
    strcpy(pp1.name,"will");
    pp1.age=30;

    print(pp1);

    struct Persion pp2;
    pp2.name=malloc(sizeof(char)*60);
#if 0
//    直接赋值导致了pp2中name的内存地址指向了pp1,下边的释放内存就属于二次释放会宕掉
//    结构体内部有指针并且指向堆空间,直接赋值会产生两个问题:1.同一块控件会被释放两次;2.内存泄漏
    pp2=pp1;
    print(pp2);

    if(pp2.name!=NULL)
    {
        free(pp2.name);
        pp2.name==NULL;
    }

    if(pp1.name!=NULL)
    {
        free(pp1.name);
        pp1.name==NULL;
    }
#endif // 0

//      标准拷贝两步:1.释放原来控件并且开辟与之对应的大小;2,赋值
    if(pp2.name!=NULL)
    {
        free(pp2.name);
        pp2.name=NULL;
    }
    pp2.name=malloc(strlen(pp1.name)+1);
    strcpy(pp2.name,pp1.name);
    pp2.age=pp1.age+1;

    print(pp2);
}


