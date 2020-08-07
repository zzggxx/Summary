#include <stdio.h>

/*结构体写入到文件中*/

struct students
{
    char name[20];
    int age;
    char sex;
    char tel[15];
};

//别名
typedef struct students stu;

void file_struct_write()
{
    stu s[5]=
    {
        {"will",20,"M","13992558334"},
        {"will",21,"M","13992558335"},
        {"will",22,"M","13992558336"},
        {"will",23,"M","13992558337"},
        {"will",24,"M","13992558338"}
    };

    FILE* fp=fopen("E://c_project//b.txt","w");
    if(!fp)
    {
        return -1;
    }
    //一次性写入5个,一次性写一个分五次都可以
//    fwrite(s,sizeof(stu),5,fp);
    for(int i=0;i<5;i++)
    {
        fwrite(&s[i],sizeof(stu),1,fp);
    }

    fclose(fp);
}

void file_struct_read()
{
    FILE* fp=fopen("E://c_project//b.txt","r");
    stu buf[5];

    printf("%d\n",sizeof(stu));

//    一次性全部读取
//    fread(buf,sizeof(stu),5,fp);
    int i=0;
    while(!feof(fp))
    {
        fread(&buf[i],sizeof(stu),1,fp);
        i++;
    }

    for(int i=0;i<5;i++)
    {
        stu temp = buf[i];
        printf("name=%s,age=%d,sex=%c,tel=%s\n",temp.name,temp.age,temp.sex,temp.tel);
    }
}
