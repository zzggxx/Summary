#include <stdio.h>
#include <stdlib.h>

int method_fputs()
{
    FILE* fp=fopen("E://c_project//a.txt","a+");//ĩβ׷��ģʽ��
    if(fp==NULL)
    {
        printf("file open failed");
        return -1;
    }
    //ָ������
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

    //fgets(),��һ������Ҫ��һ��ָ��,���ǵ�ַ,ֱ��д����,ϵͳ���ٺ��ͷ�,Ҳ�����ֶ����ٺ��ͷ�
    //��һ��:ϵͳ���ٺ��ͷ�
//    char ch[20];
//    while(!feof(fp))
//    {
//        fgets()
//        fgets(ch,3,fp);   //���������з�,����-1,�ļ�������
//        printf("%s\n",ch);
//    }

    //�ڶ���:�Լ����ٺ��ͷ�
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
