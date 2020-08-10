#include <stdio.h>
#include <stdlib.h>

//  可以快速的修改INT的类型
typedef unsigned int INT;

struct persion
{
    char name[20];
    int age;
};
typedef struct persion Persion;

typedef struct persion11
{
    char name[20];
    int age;
} Persion11;

typedef char* PCHAR;

void test()
{
//    第一种声明方式
    Persion p;
    strcpy(p.name,"will");
    p.age=30;
    printf("p.name=%s,p.age=%d\n",p.name,p.age);
//    第二种声明方式
    Persion11 p11;

    INT a=9;

//    以下方式定义的之后p1是char*,而p2是char类型
    char* p1,p2;
//    先进行typedef定义然后再声明,以下两个都是char*类型
    PCHAR p3,p4;
}
