#include <stdio.h>

int method_fwrite()
{
    FILE* fp=fopen("E://c_project//a.txt","w");
    if(!fp)
    {
        printf("file open failed!");
        return -1;
    }

    int intt[]={1111,22222222,33333333,444,5555,6666,77,8,9,10};
    //����:1.���ָ��,��ʾ��һ�ε�ַ(��ַ����������);2.���Сsizeof(int);3.�����;4.�ļ�ָ��
    fwrite(intt,sizeof(int),5,fp);      //д����һ�ѵ�����,���ǲ�Ӱ�������
    fclose(fp);
    return 0;
}

void method_fread()
{
    FILE* fp=fopen("E://c_project//a.txt","r");
    int intt[10]={0};

    //ֻ�ܶ���5555,��Ϊֻд��������,�����ϱߵ����鲻��ʼ����߾ͻ����
//    fread(intt,sizeof(int),6,fp);

    //һ�ζ�һ��int,�ϱ���һ�ζ�6��
    int i=0;
    while(!feof(fp))
    {
        fread(&intt[i++],sizeof(int),1,fp);
    }

    for(int i=0;i<10;i++)
    {
        printf("%d\n",*(intt+i));
    }
}


