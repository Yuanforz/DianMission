#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool read(int *pa,int *pb,int *pc,FILE *pfile);

int main()
{
    FILE *pf=NULL;
    errno_t error=fopen_s(&pf,"test.txt","r");
    if(errno)
    {
        printf("Error with opening the file.\n");
        return error;
    }
    int a,b,c;//from floor a to floor b when elevator stopping at floor c.
    if(!read(&a,&b,&c,pf))
    {
        printf("Error with reading file.Please check the file.\n");
        return -1;
    }
    printf("%d\t0\t0\n",c);
    printf("%d\t%d\t1\n",a,a-c);
    printf("%d\t%d\t0\n",b,2*a-b-c);
    return 0;
}

bool read(int *pa,int *pb,int *pc,FILE *pfile)
{
    int success=0;
    success=fscanf_s(pfile,"%d %d %d",pa,pb,pc);
    if(success!=3)
        return false;
    else
        return true;
}
