#include <stdio.h>

struct Persion
{
    char name[20];
    int age;
};

void printPersions(struct Persion* persions,int len)
{
    for(int i=0; i<len; i++)
    {
        printf("%s %d\n",persions[i].name,persions[i].age);
    }
}

int main()
{
//   栈上创建结构体数组
    struct Persion persions[]=
    {
        {
            "will18",18
        },
        {
            "will19",19
        },
        {
            "will20",20
        },
        {
            "will21",21
        }
    };
    int len=sizeof(persions)/sizeof(persions[0]);
    printPersions(persions,len);

//    堆上创建结构体数组
    struct Persion* persionss=(struct Persion*)malloc(sizeof(struct Persion)*6);
    for(int i=0;i<6;i++)
    {
        strcpy(persionss[i].name,"will");
        persionss[i].age=i;
    }
    printPersions(persionss,len);

}


