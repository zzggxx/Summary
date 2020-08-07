#include <stdio.h>

void method_fprintf()
{
    FILE* fp=fopen("E://c_project//a.txt","w");
    if(!fp)
    {
        printf("file open failed!");
        return -1;
    }
    fprintf(fp,"%d %d %d\n",1,2,3);
    fclose(fp);
}

void method_fscanf()
{
    FILE* fp=fopen("E://c_project//a.txt","r");
    int a;
    int b;
    int c;
    fscanf(fp,"%d %d %d\n",&a,&b,&c);
    printf("%d%d%d\n",a,b,c);
}
