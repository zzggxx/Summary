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
    //参数:1.完成指针,表示了一段地址(地址包含有数据);2.块大小sizeof(int);3.块个数;4.文件指针
    fwrite(intt,sizeof(int),5,fp);      //写出了一堆的乱码,但是不影响读出来
    fclose(fp);
    return 0;
}

void method_fread()
{
    FILE* fp=fopen("E://c_project//a.txt","r");
    int intt[10]={0};

    //只能读到5555,因为只写到那里了,若是上边的数组不初始化后边就会随机
//    fread(intt,sizeof(int),6,fp);

    //一次读一个int,上边是一次读6个
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


