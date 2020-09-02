#include <stdio.h>
#include <stdlib.h>

void array_01()
{
    int int_arr[]={1,3,5,4,2};
    for(int i=0;i<sizeof(int_arr)/sizeof(int_arr[0]);i++)
    {
        printf("%d\n",int_arr[i]);
    }
}

void array_02()
{
    char char_arr[]={'a','b','d'};
    for(int i=0;i<sizeof(char_arr)/sizeof(char_arr[0]);i++)
    {
        printf("%c\n",char_arr[i]);
    }
}
