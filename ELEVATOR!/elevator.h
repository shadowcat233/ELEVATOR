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
#define INOUT_TIME 24*T
#define CANCLE_CLOSE_TIME 10*T
#define DELAY_TIME 300*T //在某层静止的超时时间
#define TEST_TIME 40*T //有人进出时，电梯每隔40t测试一次，若无人进出，则关门
#define ACCELERATE_TIME 15*T    //加速时间
#define UP_TIME    51*T    //上升时间
#define DOWN_TIME 61*T    //下降时间
#define UP_DEC_TIME 13*T      //上升减速
#define DOWN_DEC_TIME 22*T    //下降减速

typedef enum Status
{
    NO, YES, OVERFLOW
}Status;
#define T  1  //单位时间
#define MOVE_TIME 25*T //人的进出时间
#define MAX_QUEUE_SIZE 10 //队列的最长长度
#define MAX_RUN_TIME 5000*T

int CurrentTime;

typedef struct {
    int id;//人的标志
    int InFloor;//该人进入哪层楼
    int OutFloor;//他要去哪层楼
    int GiveupTime;//他能容忍的等候时间
    int InterTime; //下一人进入系统的时间间隔
}People, * PLink;


#define MAXFLOOR 5
#define MAXSIZE 8
//排队队列（优先队列）



typedef PLink DataType;
typedef struct Node
{
    DataType data;
    struct Node* next;
}QNode;

typedef struct Queue
{
    QNode* head;
    QNode* tail;
}Queue;

Queue AllFloor[MAXFLOOR * 2];

//电梯乘客栈
typedef struct
{
    PLink* person;
    int top;
    int size;
}Stack;



void QueueInit(Queue* pq);
void QueuePush(Queue* pq, DataType x);
void QueuePop(Queue* pq);
DataType QueueHead(Queue* pq);
bool QueueEmpty(Queue* pq);

void AllQueueInit();


//初始化
Status InitStack(Stack* S, int size);
//出栈
Status Pop(Stack* S, PLink* e);
//push
Status Push(Stack* S, People* e);
//栈S是否为空
Status IsEmpty_S(Stack* S);
//栈S是否满
//Status IsFull_S(Stack S);
//打印栈的内容
void PrintStack(Stack S);
void PrintTime();


//电梯的方向
typedef enum State
{
    GoingDown,//
    GoingUp,//
}State;

/*
 *电梯状态
 */
typedef enum Condition
{
    Arrive,//等候
    Opening,//开门中
    Opened,//已经开门
    Closing,//正在关门
    Closed,//关门
    Moving,//移动
    Accelerate,
    Decelerate,//减速
    None,//无
}Condition;


typedef struct Elevator {
    State state;//电梯当前的方向
    Condition condition;//电梯当前进行的活动
    bool CallUp[MAXFLOOR];//电梯外向上的按钮
    bool CallDown[MAXFLOOR];//电梯外向下的按钮
    bool CallCar[MAXFLOOR];//电梯内的按钮
    int floor;//当前的楼层
   // Queue** w_q;//每一层的排队队列,第一组为上去的人，第二组为下去的人
    Stack* Stack[MAXFLOOR];//电梯内的人，按目标层数入栈
    int count;//运行的计数器
    int peopleNum;//用户的数量
    bool InOut;//标志有人进出
    bool valid_idle;//标志是否停留超时
    bool NoOneInOut;//标志门开但无人进出
    bool IsClosing;//标志是否正在关门
    bool stay;
}Elevator;

Elevator e;

int people_id;
int serve_people;
Status AddPeople();
void DePeople(int i);
void LackPatience(int i);
void DeAllFloor();

void InitElevator();
bool IsNeedOpen();
bool IsNeedUp();
bool IsNeedDown();
bool IsNeedStop();
//Condition NextCondition(Elevator e);
void GoUpOneFloor();
void GoDownOneFloor();
Status ElevatorIn(int i);
Status ElevatorOut();
void CallOutside();
void CallInside();
State ChangeDirection();

typedef struct Activity {
    int startTime;//动作结束的时间
    Condition(*fn)(void);//执行的动作
    struct Activity* next;//下一个动作
}Activity;

Activity* activity;

void init();
void run();
Condition NextCondition();
void AddActivity(int startTime, Condition(*fn)(void));
void Act();
Condition Close(void);
Condition Out(void);
Condition In(void);
char* ConditionToString(Condition con);

#endif /* elevator_hpp */