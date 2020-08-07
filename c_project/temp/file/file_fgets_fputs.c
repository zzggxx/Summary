#include <stdio.h>
#include <stdlib.h>

int method_fputs()
{
    FILE* fp=fopen("E://c_project//a.txt","a+");//末尾追加模式打开
    if(fp==NULL)
    {
        printf("file open failed");
        return -1;
    }
    //指针数组
    char* cha[]= {"123\n","456\n","789\n"};
    for(int i=0; i<3; i++)
    {
        fputs(cha[i],fp);
    }
    fclose(fp);
    return 0;
}

int method_fget()
{

    FILE* fp=fopen("E://c_project//a.txt","r");
    if(fp==NULL)
    {
        return -1;
    }

    //fgets(),第一参数需要了一个指针,就是地址,直接写数组,系统开辟和释放,也可以手动开辟和释放
    //第一种:系统开辟和释放
//    char ch[20];
//    while(!feof(fp))
//    {
//        fgets()
//        fgets(ch,3,fp);   //当遇到换行符,长度-1,文件结束符
//        printf("%s\n",ch);
//    }

    //第二种:自己开辟和释放
    int len=sizeof(char)*1024*0.1;
    char* ch=malloc(len);
    memset(ch,0,len);
    while(!feof(fp))
    {
        fgets(ch,len,fp);
        printf("%s\n",ch);
        memset(ch,0,len);
    }
    if(ch)
    {
        free(ch);
    }
    ch=NULL;
    free(ch);

    fclose(fp);
    return 0;
}
