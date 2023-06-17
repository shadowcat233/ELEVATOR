#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>
#include<assert.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_PEOPLE 10 //电梯内的最多人

#define OC_TIME 20*T //开关门的时间
#define START_CHECK_TIME 75*T//开始检测是否需要关门的时间
#define INOUT_TIME 24*T//为了保证运行时时间正确，乘客进出时间比设定少1
#define CANCLE_CLOSE_TIME 10*T//取消关门所花费时间
#define DELAY_TIME 300*T//在某层静止的超时时间
#define TEST_TIME 40*T//有人进出时，电梯每隔40t测试一次，若无人进出，则关门
#define ACCELERATE_TIME 15*T//加速时间
#define UP_TIME 51*T//上升时间
#define DOWN_TIME 61*T//下降时间
#define UP_DEC_TIME 13*T//上升减速（比设定少1）
#define DOWN_DEC_TIME 22*T//下降减速（比设定少1）

typedef enum Status
{
    NO, YES, OVERFLOW
}Status;
#define T 1//单位时间
#define MAX_RUN_TIME 30000*T//最长运行时间

int CurrentTime;//当前时间

//乘客
typedef struct 
{
    int id;//乘客编号
    int InFloor;//进入时的楼层
    int OutFloor;//要去的楼层
    int GiveupTime;//能容忍的等候时间
    int InterTime; //下一人进入系统的时间间隔
}People, * PLink;


#define MAXFLOOR 5//最高楼层

//队列中数据元素类型：指向PEOPLE的指针
typedef PLink DataType;

//排队队列结点
typedef struct QNode
{
    DataType data;
    struct QNode* next;
}QNode;

//排队队列
typedef struct Queue
{
    QNode* head;
    QNode* tail;
}Queue;

//所有楼层队伍，其中偶数表示上行队伍，奇数表示下行队伍
Queue AllFloor[MAXFLOOR * 2];

//电梯乘客栈
typedef struct
{
    PLink* person;
    int top;
    int size;
}Stack;


//队列初始化
void QueueInit(Queue* pq);
//入队
void QueuePush(Queue* pq, DataType x);
//出队
void QueuePop(Queue* pq);
//取队头元素
DataType QueueHead(Queue* pq);
//队列是否为空
bool QueueEmpty(Queue* pq);

//初始化所有队列
void AllQueueInit();


//初始化栈
Status InitStack(Stack* S, int size);
//出栈
Status Pop(Stack* S, PLink* e);
//入栈
Status Push(Stack* S, People* e);
//栈是否为空
Status IsEmpty_S(Stack* S);
//打印栈的内容
void PrintStack(Stack S);

//打印当前时间
void PrintTime();


//电梯的方向
typedef enum State
{
    GoingDown,//下行
    GoingUp,//上行
}State;


//电梯状态
typedef enum Condition
{
    Arrive,//等候
    Opening,//开门中
    Opened,//已经开门
    Closing,//正在关门
    Closed,//关门
    Moving,//移动
    Accelerate,//加速
    Decelerate,//减速
}Condition;


//电梯
typedef struct Elevator 
{
    State state;//电梯当前的方向
    Condition condition;//电梯当前进行的活动
    bool CallUp[MAXFLOOR];//电梯外向上的按钮
    bool CallDown[MAXFLOOR];//电梯外向下的按钮
    bool CallCar[MAXFLOOR];//电梯内的按钮
    int floor;//当前的楼层
    Stack* Stack[MAXFLOOR];//电梯内的人，按目标层数入栈
    int count;//倒计时
    int peopleNum;//用户的数量
    bool InOut;//标志有人进出
    bool valid_idle;//标志是否停留超时
    bool NoOneInOut;//标志门开但无人进出
    bool IsClosing;//标志是否正在关门
    bool stay;//长久地停在1楼
}Elevator;

//本程序中的电梯
Elevator e;

int people_id;//乘客编号
int serve_people;//已服务的乘客数

//添加一个乘客
Status AddPeople();
//删除一个乘客
void DePeople(int i);
//删除某层某状态的失去耐心的乘客
void LackPatience(int i);
//删除所有层失去耐心的乘客
void DeAllFloor();

//初始化电梯
void InitElevator();
//是否需要开门
bool IsNeedOpen();
//是否需要上行
bool IsNeedUp();
//是否需要下行
bool IsNeedDown();
//是否需要停止移动
bool IsNeedStop();
//向上一层
void GoUpOneFloor();
//向下一层
void GoDownOneFloor();
//一名乘客进电梯
Status ElevatorIn(int i);
//一名乘客出电梯
Status ElevatorOut();
//void CallOutside();
//void CallInside();
//判断电梯即将移动方向
State ChangeDirection();
//回复电梯楼层
int GetFloor(int floor);

//活动（包括乘客进电梯，乘客出电梯，检查是否关门）
typedef struct Activity 
{
    int startTime;//活动开始的时间
    Condition(*fn)(void);//需进行的活动（程序指针）
    struct Activity* next;//下一个活动
}Activity;

//本程序中活动的指针
Activity* activity;


//初始化程序
void init();
//运行程序
void run();
//判断电梯下一个状态
Condition NextCondition();
//添加活动
void AddActivity(int startTime, Condition(*fn)(void));
//进行活动
void Act();
//检查是否需要关门
Condition Close(void);
//乘客出电梯
Condition Out(void);
//乘客进电梯
Condition In(void);
char* ConditionToString(Condition con);

#endif /* elevator_hpp */