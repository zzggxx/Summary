#include <stdio.h>

/*�ṹ��д�뵽�ļ���*/

struct students
{
    char name[20];
    int age;
    char sex;
    char tel[15];
};

//����
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
    //һ����д��5��,һ����дһ������ζ�����
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

//    һ����ȫ����ȡ
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
