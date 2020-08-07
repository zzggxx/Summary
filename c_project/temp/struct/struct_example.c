#include <stdio.h>

struct stu
{
    char name[20];
    int age;
};

void struct_example()
{
    struct stu stu1={"will",18};
    printf("name=%s,age=%d\n",stu1.name,stu1.age);
}
