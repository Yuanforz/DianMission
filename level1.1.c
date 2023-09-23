#include <stdio.h>
#include <stdlib.h>

int main()
{
    int a,b,c;//from floor a to floor b when elevator stopping at floor c.
    scanf("%d %d %d",&a,&b,&c);
    printf("%d\t0\t0\n",c);
    printf("%d\t%d\t1\n",a,a-c);
    printf("%d\t%d\t0\n",b,2*a-b-c);
    getchar();//pause
    return 0;
}
