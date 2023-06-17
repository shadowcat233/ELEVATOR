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
#define INOUT_TIME 24*T//Ϊ�˱�֤����ʱʱ����ȷ���˿ͽ���ʱ����趨��1
#define CANCLE_CLOSE_TIME 10*T//ȡ������������ʱ��
#define DELAY_TIME 300*T//��ĳ�㾲ֹ�ĳ�ʱʱ��
#define TEST_TIME 40*T//���˽���ʱ������ÿ��40t����һ�Σ������˽����������
#define ACCELERATE_TIME 15*T//����ʱ��
#define UP_TIME 51*T//����ʱ��
#define DOWN_TIME 61*T//�½�ʱ��
#define UP_DEC_TIME 13*T//�������٣����趨��1��
#define DOWN_DEC_TIME 22*T//�½����٣����趨��1��

typedef enum Status
{
    NO, YES, OVERFLOW
}Status;
#define T 1//��λʱ��
#define MAX_RUN_TIME 30000*T//�����ʱ��

int CurrentTime;//��ǰʱ��

//�˿�
typedef struct 
{
    int id;//�˿ͱ��
    int InFloor;//����ʱ��¥��
    int OutFloor;//Ҫȥ��¥��
    int GiveupTime;//�����̵ĵȺ�ʱ��
    int InterTime; //��һ�˽���ϵͳ��ʱ����
}People, * PLink;


#define MAXFLOOR 5//���¥��

//����������Ԫ�����ͣ�ָ��PEOPLE��ָ��
typedef PLink DataType;

//�ŶӶ��н��
typedef struct QNode
{
    DataType data;
    struct QNode* next;
}QNode;

//�ŶӶ���
typedef struct Queue
{
    QNode* head;
    QNode* tail;
}Queue;

//����¥����飬����ż����ʾ���ж��飬������ʾ���ж���
Queue AllFloor[MAXFLOOR * 2];

//���ݳ˿�ջ
typedef struct
{
    PLink* person;
    int top;
    int size;
}Stack;


//���г�ʼ��
void QueueInit(Queue* pq);
//���
void QueuePush(Queue* pq, DataType x);
//����
void QueuePop(Queue* pq);
//ȡ��ͷԪ��
DataType QueueHead(Queue* pq);
//�����Ƿ�Ϊ��
bool QueueEmpty(Queue* pq);

//��ʼ�����ж���
void AllQueueInit();


//��ʼ��ջ
Status InitStack(Stack* S, int size);
//��ջ
Status Pop(Stack* S, PLink* e);
//��ջ
Status Push(Stack* S, People* e);
//ջ�Ƿ�Ϊ��
Status IsEmpty_S(Stack* S);
//��ӡջ������
void PrintStack(Stack S);

//��ӡ��ǰʱ��
void PrintTime();


//���ݵķ���
typedef enum State
{
    GoingDown,//����
    GoingUp,//����
}State;


//����״̬
typedef enum Condition
{
    Arrive,//�Ⱥ�
    Opening,//������
    Opened,//�Ѿ�����
    Closing,//���ڹ���
    Closed,//����
    Moving,//�ƶ�
    Accelerate,//����
    Decelerate,//����
}Condition;


//����
typedef struct Elevator 
{
    State state;//���ݵ�ǰ�ķ���
    Condition condition;//���ݵ�ǰ���еĻ
    bool CallUp[MAXFLOOR];//���������ϵİ�ť
    bool CallDown[MAXFLOOR];//���������µİ�ť
    bool CallCar[MAXFLOOR];//�����ڵİ�ť
    int floor;//��ǰ��¥��
    Stack* Stack[MAXFLOOR];//�����ڵ��ˣ���Ŀ�������ջ
    int count;//����ʱ
    int peopleNum;//�û�������
    bool InOut;//��־���˽���
    bool valid_idle;//��־�Ƿ�ͣ����ʱ
    bool NoOneInOut;//��־�ſ������˽���
    bool IsClosing;//��־�Ƿ����ڹ���
    bool stay;//���õ�ͣ��1¥
}Elevator;

//�������еĵ���
Elevator e;

int people_id;//�˿ͱ��
int serve_people;//�ѷ���ĳ˿���

//���һ���˿�
Status AddPeople();
//ɾ��һ���˿�
void DePeople(int i);
//ɾ��ĳ��ĳ״̬��ʧȥ���ĵĳ˿�
void LackPatience(int i);
//ɾ�����в�ʧȥ���ĵĳ˿�
void DeAllFloor();

//��ʼ������
void InitElevator();
//�Ƿ���Ҫ����
bool IsNeedOpen();
//�Ƿ���Ҫ����
bool IsNeedUp();
//�Ƿ���Ҫ����
bool IsNeedDown();
//�Ƿ���Ҫֹͣ�ƶ�
bool IsNeedStop();
//����һ��
void GoUpOneFloor();
//����һ��
void GoDownOneFloor();
//һ���˿ͽ�����
Status ElevatorIn(int i);
//һ���˿ͳ�����
Status ElevatorOut();
//void CallOutside();
//void CallInside();
//�жϵ��ݼ����ƶ�����
State ChangeDirection();
//�ظ�����¥��
int GetFloor(int floor);

//��������˿ͽ����ݣ��˿ͳ����ݣ�����Ƿ���ţ�
typedef struct Activity 
{
    int startTime;//���ʼ��ʱ��
    Condition(*fn)(void);//����еĻ������ָ�룩
    struct Activity* next;//��һ���
}Activity;

//�������л��ָ��
Activity* activity;


//��ʼ������
void init();
//���г���
void run();
//�жϵ�����һ��״̬
Condition NextCondition();
//��ӻ
void AddActivity(int startTime, Condition(*fn)(void));
//���л
void Act();
//����Ƿ���Ҫ����
Condition Close(void);
//�˿ͳ�����
Condition Out(void);
//�˿ͽ�����
Condition In(void);
char* ConditionToString(Condition con);

#endif /* elevator_hpp */