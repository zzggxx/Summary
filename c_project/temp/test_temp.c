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
//    ֱ�Ӹ�ֵ������pp2��name���ڴ��ַָ����pp1,�±ߵ��ͷ��ڴ�����ڶ����ͷŻ�崵�
//    �ṹ���ڲ���ָ�벢��ָ��ѿռ�,ֱ�Ӹ�ֵ�������������:1.ͬһ��ؼ��ᱻ�ͷ�����;2.�ڴ�й©
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

//      ��׼��������:1.�ͷ�ԭ���ؼ����ҿ�����֮��Ӧ�Ĵ�С;2,��ֵ
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


