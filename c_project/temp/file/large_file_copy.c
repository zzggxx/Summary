#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#define MAXSIZE 1024*1024*10

/*大文件拷贝(运行出错了不知为何)*/

void large_file_copy()
{
    unsigned int start_time= time(NULL);

    FILE* fpread=fopen("E://c_project//test.mp4","rb");
    FILE* fpwrite=fopen("E://c_project//test1.mp4","wb");
    if(!fpread || !fpwrite)
    {
        printf("file open failed");
        return;
    }
    struct stat* s=NULL;
    stat("E://c_project//test.mp4",s);
    int len;
    if(s->st_size<MAXSIZE)
    {
        len=s->st_size;
    }
    else
    {
        len=MAXSIZE;
    }
    char* ch = (char*)malloc(sizeof(char)*len);
    while(!feof(fpread))
    {
        memset(ch,0,len);
        int availaberead=fread(ch,sizeof(char),len,fpread);
        fwrite(ch,1,availaberead,fpwrite);
    }
    fclose(fpread);
    fclose(fpwrite);

    free(ch);

    unsigned int end_time=time(NULL);
	printf("花费时间：%d(s)\n",end_time-start_time);
}



