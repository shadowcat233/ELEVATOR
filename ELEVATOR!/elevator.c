#include"elevator.h"




void InitElevator() {
    e.condition = Closed;
    e.state = GoingUp;
    e.floor = 1;
    e.peopleNum = 0;
    e.count = 0;
    e.valid_idle = true;
    e.IsClosing = false;
    e.stay = false;
    for (int i = 0; i < (MAXFLOOR); ++i) {
        e.Stack[i] = (Stack*)malloc(sizeof(Stack));
        InitStack(e.Stack[i], MAX_PEOPLE);
        e.CallUp[i] = e.CallDown[i] = e.CallCar[i] = false;
    }
}
//�ж��Ƿ�Ҫ����
bool IsNeedOpen() {
    if (e.stay && e.condition == Arrive)
    {
        e.stay = false;
        return false;
    }
    if(e.peopleNum<MAX_PEOPLE)
    {
        if (e.CallCar[e.floor]) return true;
        if (e.CallDown[e.floor] && e.state == GoingDown)return true;
        if (e.CallUp[e.floor] && e.state == GoingUp)return true;
    }
    return false;
}

//�ж��Ƿ����ƶ����߲������
bool IsNeedUp() {
    for (int i = e.floor + 1; i < MAXFLOOR; i++)
        if (e.CallCar[i] || e.CallDown[i] || e.CallUp[i]) return true;
    if (e.condition == Opened && e.CallUp[e.floor])return true;
    if (e.condition != Moving && e.condition != Accelerate && e.CallUp[e.floor])return true;
    return false;
}
//�ж��Ƿ����ƶ����Ͳ������
bool IsNeedDown() {
    for (int i = 0; i <= e.floor - 1; i++)
        if (e.CallCar[i] || e.CallDown[i] || e.CallUp[i]) return true;
    if (e.condition == Opened && e.CallDown[e.floor])return true;
    if (e.condition != Moving && e.condition != Accelerate && e.CallDown[e.floor])return true;
    return false;
}
//�жϵ����Ƿ�Ҫֹͣ
bool IsNeedStop() {
    if (e.floor >= MAXFLOOR || e.floor < 0)return true;
    //�Ƿ���һ������Ҫȥ
    if (e.CallCar[e.floor]) return true;
    //û��ȥ�����Ƿ���������һ�㰴�˵��ݰ�ť
    if ((e.state == GoingUp && e.CallUp[e.floor]) || (e.state == GoingDown && e.CallDown[e.floor]))
        return true;
    if (!IsNeedDown() && !IsNeedUp())
        return true;
    if (e.state == GoingDown && e.CallUp[e.floor] && !IsNeedDown()) {
        e.state = GoingUp;
        return true;
    }
    if (e.state == GoingUp && e.CallDown[e.floor] && !IsNeedUp()) {
        e.state = GoingDown;
        return true;
    }
    //ȷ���Ƿ�ص��˱��ݲ�
    //if (e.stay && e.floor == 1)return true;
    return false;
}
void GoUpOneFloor()
{
    e.floor++;
  //  printf("����������%d��,", GetFloor(e.floor));
  //  PrintTime();
}
void GoDownOneFloor()
{
    e.floor--;
   // printf("�����½���%d��,", GetFloor(e.floor));
    //PrintTime();
}
Status ElevatorIn(int i)
{
    People* p = QueueHead(&AllFloor[i]);
    Push(e.Stack[p->OutFloor], p);
    DePeople(i);
    e.peopleNum++;
    e.CallCar[p->OutFloor] = true;
   // printf("�˿�%d�������,", p->id);
  //  PrintTime();
    return YES;
}
Status ElevatorOut()
{
    PLink* p = (People**)malloc(sizeof(People*));
    if (!p)exit(-1);
    Pop(e.Stack[e.floor], p);
    //if (!(*p)->id)return NO;
    e.peopleNum--;
   /// printf("�˿�%d�뿪����,", (*p)->id);
    //PrintTime();
    return YES;
}
int GetFloor(int floor)
{
    if (!floor)return -1;
    else return floor;
}
/*
void CallOutside()
{
    int i;
    for (i = 0; i < MAXFLOOR; i++)
    {
        if (!QueueEmpty(&AllFloor[2 * i + 1]))e.CallUp[i] = true;
        else e.CallUp[i] = false;
        if (!QueueEmpty(&AllFloor[2 * i]))e.CallDown[i] = true;
        else e.CallDown[i] = false;
    }
}
void CallInside()
{
    int i;
    for (i = 0; i < MAXFLOOR; i++)
        if (!IsEmpty_S(e.Stack[i]))e.CallCar[i] = true;
        else e.CallCar[i] = false;

}*/

State ChangeDirection()
{
    int i, down, up;
    for (i = 0; i < e.floor; i++)
        if (e.CallCar[i])return GoingDown;
    for (i = e.floor + 1; i < MAXFLOOR; i++)
        if (e.CallCar[i])return GoingUp;
    if (e.floor == MAXFLOOR - 1)return GoingDown;
    if (e.floor == 0)return GoingUp;
    if (e.CallCar[e.floor])return e.state;
    down = IsNeedDown();
    up = IsNeedUp();
    //if (e.condition == Opened && e.CallUp[e.floor]&&!down)return GoingUp;
    //if (e.condition == Opened && e.CallDown[e.floor] && !up)return GoingDown;
    if (down && !up)return GoingDown;
    if (up && !down)return GoingUp;
    if ((e.condition == Moving || e.condition == Accelerate) && up && down)return e.state;

    //if (up && down)return e.state;

    //if (e.CallUp[e.floor] && !e.CallDown[e.floor])return GoingUp;
    //if (!e.CallUp[e.floor] && e.CallDown[e.floor])return GoingDown;
    if (e.CallUp[e.floor] && e.CallDown[e.floor])
    {
        People* p_up, * p_down;
        p_down = QueueHead(&AllFloor[2 * e.floor]);
        p_up = QueueHead(&AllFloor[2 * e.floor + 1]);
        return p_down->id < p_up->id ? GoingDown : GoingUp;
    }
    return e.state;
}
