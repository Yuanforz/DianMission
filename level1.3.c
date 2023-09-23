#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CLIENT 100
#define MAX_CAPABILITY 4
#define NUM_FLOOR 20
#define MAX_CHARINPUT 100
#define MAX_TRAIL 1000
#define MAX_SHOWINLINE 15
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))


int readFile(int *first_array,int *second_array,int *number);
void rearragePos(int *infloor,int *outfloor,int number);
int AllocateEle(int *infloor,int *outfloor,int number,int* trail);

int main()
{

    int callFloor[MAX_CLIENT]={0};
    int goalFloor[MAX_CLIENT]={0};
    int clientnum=0;
    int trail[MAX_TRAIL]={0};
    if(readFile(callFloor,goalFloor,&clientnum))
    {
        return -1;
    }
    if(clientnum==0)
    {
        printf("No client detected,quitting automatically.\n");
        return 0;
    }
    printf("Please make sure there be %d client waiting for elevator.\n",clientnum);
    rearragePos(callFloor,goalFloor,clientnum);
    for(int i=0;i<clientnum;i++)
    {
        printf("the %3dth person from %2d to %2d.\n",i+1,callFloor[i],goalFloor[i]);
    }
    printf("Total %d time used.\n",AllocateEle(callFloor,goalFloor,clientnum,trail));
    printf("The trail of the elevator is recorded as below(Only show the first %d records).\n",MAX_TRAIL);
    printf(" 1");
    for(int i=0;i<MAX_TRAIL;i++)
    {
        if(trail[i]==0)
            break;
        printf(" ->%2d",trail[i]);
        if(i%MAX_SHOWINLINE==MAX_SHOWINLINE-1)
            printf("\n");
    }
    return 0;
}

int readFile(int *first_array,int *second_array,int *number)//return 0 means working successfully.
{
    FILE *pf=NULL;
    errno_t error=fopen_s(&pf,"test2.txt","r");
    if(errno)
    {
        printf("Error with opening the file.\n");
        return error;
    }
    char ch[MAX_CHARINPUT]={0};

    int count=0;

    while(fgets(ch,MAX_CHARINPUT,pf)!=NULL)
    {
        if(ch[0]!='#')
        {
            if((sscanf(ch,"%d %d",first_array+count,second_array+count)!=2)||
               (first_array[count]<1)||(first_array[count]>NUM_FLOOR)||
               (second_array[count]<1)||(second_array[count]>NUM_FLOOR)||
               (first_array[count]==second_array[count]))
            {
                printf("Warning:Existing illegal input.Ignored it.Please check README.md\n");
                count--;
            }
            count++;
            if(count>=MAX_CLIENT)
            {
                *number=count;
                printf("Warning:Exceeding max capability.Terminate input.\n");
                return 0;
            }
        }
    }
    *number=count;
    return 0;
}

void rearragePos(int *infloor,int *outfloor,int number)
{
    int newin[MAX_CLIENT]={0};
    int newout[MAX_CLIENT]={0};
    int count=0;
    for(int i=1;i<=NUM_FLOOR;i++)
        for(int j=0;j<number;j++)
            if(infloor[j]==i)
            {
                newin[count]=i;
                newout[count]=outfloor[j];
                count++;
            }
    for(int i=0;i<number;i++)
    {
        infloor[i]=newin[i];
        outfloor[i]=newout[i];
    }
}




int AllocateEle(int *infloor,int *outfloor,int number,int *trail)
{
    int preFloor=1;
    int currentFloor=1;
    int target[MAX_CAPABILITY]={0};
    int currentClientNum=0;
    int totaltime=0;
    int count=0;
    int currentTarget=0;
    int inperson=0;
    int outperson=0;
    bool currentWay=0;//0 means up when 1 means down
    trail[0]=currentFloor;
    while(1)
    {
        count++;

        //catch the client in
        inperson=0;
        for(int i=0;i<number;i++)
            if(infloor[i]==currentFloor)
            {
                currentClientNum++;
                inperson++;
                target[currentClientNum-1]=outfloor[i];
                infloor[i]=0;
                outfloor[i]=0;
                if(currentClientNum>=MAX_CAPABILITY)
                    break;
            }


        //output the result
        printf("The %3dst stop:from %2d to %2d.\t\t",count,preFloor,currentFloor);
        printf("Cost %3d time with %d in the elevator now.(-%d +%d)\n",totaltime,currentClientNum,outperson,inperson);

        //choose the target floor
        int in_goal_up=0;
        int in_goal_down=NUM_FLOOR+1;
        int out_goal_up=0;
        int out_goal_down=NUM_FLOOR+1;

        for(int i=0;i<currentClientNum;i++)
            if((in_goal_up<currentFloor)||((target[i]>currentFloor)&&(target[i]<in_goal_up)))
                in_goal_up=target[i];
        if(in_goal_up<currentFloor)
            in_goal_up=0;
        for(int i=0;i<currentClientNum;i++)
            if((in_goal_down>currentFloor)||((target[i]<currentFloor)&&(target[i]>in_goal_down)))
                in_goal_down=target[i];
        if(in_goal_down>currentFloor)
            in_goal_down=0;
        for(int i=0;i<number;i++)
            if((infloor[i]>0)&&(infloor[i]!=currentFloor))
                if((out_goal_up<currentFloor)||((target[i]>currentFloor)&&(infloor[i]<out_goal_up)))
                    out_goal_up=infloor[i];
        if(out_goal_up<currentFloor)
            out_goal_up=0;
        for(int i=0;i<number;i++)
            if((infloor[i]>0)&&(infloor[i]!=currentFloor))
                if((out_goal_down>currentFloor)||((target[i]<currentFloor)&&(infloor[i]>out_goal_down)))
                    out_goal_down=infloor[i];
        if(out_goal_down>currentFloor)
            out_goal_down=0;


        if(!(in_goal_up||in_goal_down||out_goal_up||out_goal_down))//determine to return or not
            return totaltime;

        if(currentClientNum>=MAX_CAPABILITY)
        {
            if(!currentWay)
                currentTarget=in_goal_up?in_goal_up:in_goal_down;
            else
                currentTarget=in_goal_down?in_goal_down:in_goal_up;
        }
        else{
            if(!currentWay)
            {
                if(out_goal_up||in_goal_up)
                {
                    if(out_goal_up&&in_goal_up)
                        currentTarget=min(out_goal_up,in_goal_up);
                    else
                        currentTarget=out_goal_up+in_goal_up;
                }
                else{
                    if(out_goal_down&&in_goal_down)
                        currentTarget=max(out_goal_down,in_goal_down);
                    else
                        currentTarget=out_goal_down+in_goal_down;
                }
            }
            else{
                if(out_goal_down||in_goal_down)
                {
                    if(out_goal_down&&in_goal_down)
                        currentTarget=max(out_goal_down,in_goal_down);
                    else
                        currentTarget=out_goal_down+in_goal_down;
                }
                else{
                    if(out_goal_up&&in_goal_up)
                        currentTarget=min(out_goal_up,in_goal_up);
                    else
                        currentTarget=out_goal_up+in_goal_up;
                }
            }
        }

        //move the elevator
        preFloor=currentFloor;
        currentFloor=currentTarget;
        totaltime+=abs(currentFloor-preFloor);
        if(count<MAX_TRAIL)
            trail[count]=currentFloor;

        //free the client
        outperson=0;
        int temp[MAX_CAPABILITY]={0};
        int *ptemp=temp;
        for(int i=0;i<currentClientNum;i++)
            if(target[i]!=currentFloor)
            {
                *ptemp=target[i];
                ptemp++;
            }
        outperson=currentClientNum-(ptemp-temp);
        currentClientNum=ptemp-temp;
        for(int i=0;i<currentClientNum;i++)
            target[i]=temp[i];
    }
}


















