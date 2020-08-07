#include <stdio.h>

enum colors
{
    red,
    blue=5,
    yellow,
    black=15,
    white
};

void enum_example()
{
    enum colors a,b,c;
    a=red;
    b=yellow;
    c=white;
    printf("%d %d %d\n",a,b,c);
}
