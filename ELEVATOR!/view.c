#include"elevator.h"

void goto_xy(int x, int y) 
{
    HANDLE hout;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x,y };
    SetConsoleCursorPosition(hout, pos);
}

void PrintIntroduction()
{
    goto_xy(0, 1);
    printf("###############################################################\n");
    printf("###########################电梯模拟器##########################\n");
    printf("###############################################################\n");
    goto_xy(4, 5);
    PrintTime();
    goto_xy(36, 5);
    printf("%s", ConditionToString(e.condition));
}

void DrawElevator()
{
    int i, j;
    for (i = 1; i <= 40; i++)
    {
        for (j = 0; j < 12; j += 2)
        {
            goto_xy(XSTART + i, YSTART + j);
            printf("-");
        }
    }
    for (i = 0; i < 10; i++)
    {
        goto_xy(XSTART, YSTART + i);
        printf("|");
        goto_xy(XSTART+40+1, YSTART + i);
        printf("|");
    }
    goto_xy(XSTART+1,YSTART+2*(4-e.floor)+1);
    printf("[");
    goto_xy(XSTART + 40, YSTART + 2 * (4 - e.floor)+1);
    printf("]");
    goto_xy(XSTART+2, YSTART + 2 * (4 - e.floor)+1);
    for(i=0;i<MAXFLOOR;i++)
        PrintStack(*(e.Stack[i]));
}

void DrawFloorQueue()
{
    int i;
    for (i = 0; i < MAXFLOOR; i++)
    {
        goto_xy(XSTART + 40 + 1 + 5, YSTART + 2 * (4 - i)+1);
        printf("下行：");
        PrintQueue(&AllFloor[2 * i]);
        printf("上行：");
        PrintQueue(&AllFloor[2 * i+1]);
    }
}

void View()
{
    PrintIntroduction();
    DrawElevator();
    DrawFloorQueue();
}