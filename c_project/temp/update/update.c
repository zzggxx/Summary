#include <stdio.h>
#include <stdlib.h>

//  ���Կ��ٵ��޸�INT������
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
//    ��һ��������ʽ
    Persion p;
    strcpy(p.name,"will");
    p.age=30;
    printf("p.name=%s,p.age=%d\n",p.name,p.age);
//    �ڶ���������ʽ
    Persion11 p11;

    INT a=9;

//    ���·�ʽ�����֮��p1��char*,��p2��char����
    char* p1,p2;
//    �Ƚ���typedef����Ȼ��������,������������char*����
    PCHAR p3,p4;
}
