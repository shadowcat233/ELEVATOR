#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>
#include<assert.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_PEOPLE 10 //�����ڵ������

#define OC_TIME 20*T //�����ŵ�ʱ��
#define START_CHECK_TIME 75*T//��ʼ����Ƿ���Ҫ���ŵ�ʱ��
#define INOUT_TIME 24*T
#define CANCLE_CLOSE_TIME 10*T
#define DELAY_TIME 300*T //��ĳ�㾲ֹ�ĳ�ʱʱ��
#define TEST_TIME 40*T //���˽���ʱ������ÿ��40t����һ�Σ������˽����������
#define ACCELERATE_TIME 15*T    //����ʱ��
#define UP_TIME    51*T    //����ʱ��
#define DOWN_TIME 61*T    //�½�ʱ��
#define UP_DEC_TIME 13*T      //��������
#define DOWN_DEC_TIME 22*T    //�½�����

typedef enum Status
{
    NO, YES, OVERFLOW
}Status;
#define T  1  //��λʱ��
#define MOVE_TIME 25*T //�˵Ľ���ʱ��
#define MAX_QUEUE_SIZE 10 //���е������
#define MAX_RUN_TIME 5000*T

int CurrentTime;

typedef struct {
    int id;//�˵ı�־
    int InFloor;//���˽����Ĳ�¥
    int OutFloor;//��Ҫȥ�Ĳ�¥
    int GiveupTime;//�������̵ĵȺ�ʱ��
    int InterTime; //��һ�˽���ϵͳ��ʱ����
}People, * PLink;


#define MAXFLOOR 5
#define MAXSIZE 8
//�ŶӶ��У����ȶ��У�



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

//���ݳ˿�ջ
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


//��ʼ��
Status InitStack(Stack* S, int size);
//��ջ
Status Pop(Stack* S, PLink* e);
//push
Status Push(Stack* S, People* e);
//ջS�Ƿ�Ϊ��
Status IsEmpty_S(Stack* S);
//ջS�Ƿ���
//Status IsFull_S(Stack S);
//��ӡջ������
void PrintStack(Stack S);
void PrintTime();


//���ݵķ���
typedef enum State
{
    GoingDown,//
    GoingUp,//
}State;

/*
 *����״̬
 */
typedef enum Condition
{
    Arrive,//�Ⱥ�
    Opening,//������
    Opened,//�Ѿ�����
    Closing,//���ڹ���
    Closed,//����
    Moving,//�ƶ�
    Accelerate,
    Decelerate,//����
    None,//��
}Condition;


typedef struct Elevator {
    State state;//���ݵ�ǰ�ķ���
    Condition condition;//���ݵ�ǰ���еĻ
    bool CallUp[MAXFLOOR];//���������ϵİ�ť
    bool CallDown[MAXFLOOR];//���������µİ�ť
    bool CallCar[MAXFLOOR];//�����ڵİ�ť
    int floor;//��ǰ��¥��
   // Queue** w_q;//ÿһ����ŶӶ���,��һ��Ϊ��ȥ���ˣ��ڶ���Ϊ��ȥ����
    Stack* Stack[MAXFLOOR];//�����ڵ��ˣ���Ŀ�������ջ
    int count;//���еļ�����
    int peopleNum;//�û�������
    bool InOut;//��־���˽���
    bool valid_idle;//��־�Ƿ�ͣ����ʱ
    bool NoOneInOut;//��־�ſ������˽���
    bool IsClosing;//��־�Ƿ����ڹ���
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
    int startTime;//����������ʱ��
    Condition(*fn)(void);//ִ�еĶ���
    struct Activity* next;//��һ������
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