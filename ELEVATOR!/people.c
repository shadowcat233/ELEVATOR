#include"elevator.h"

void QueueInit(Queue* pq)//��ʼ��
{
	assert(pq);
	pq->head = NULL;
	pq->tail = NULL;

}

void QueuePush(Queue* pq, DataType x)//���
{
	assert(pq);
	if (pq->head == NULL)
	{
		QNode* NewNode = (QNode*)malloc(sizeof(QNode));
		if (!NewNode)exit(-1);
		NewNode->data = x;
		NewNode->next = NULL;
		pq->head = NewNode;
		pq->tail = NewNode;
	}
	else
	{
		QNode* NewNode = (QNode*)malloc(sizeof(QNode));
		if (!NewNode)exit(-1);
		NewNode->data = x;
		NewNode->next = NULL;
		pq->tail->next = NewNode;
		pq->tail = NewNode;
	}
}

void QueuePop(Queue* pq)//����
{
	assert(pq);
	QNode* val = pq->head;
	if (pq->head == pq->tail)
	{
		pq->head = NULL;
		pq->head = NULL;
		free(val);
	}
	else
	{
		pq->head = pq->head->next;
		free(val);
	}
}

DataType QueueHead(Queue* pq)//ȡ��ͷԪ��
{
	assert(pq);
	assert(pq->head);
	return pq->head->data;
}

bool QueueEmpty(Queue* pq)//�������Ƿ�Ϊ��
{
	assert(pq);
	return pq->head == NULL;
}

void AllQueueInit()
{
	int i;
	for (i = 0; i < MAXFLOOR * 2; i++)
		QueueInit(&AllFloor[i]);
}


Status InitStack(Stack* S, int size)//��ʼ��
{
	S->person = (PLink*)malloc(sizeof(PLink) * size);
	if (NULL == S->person) { return NO; }
	S->size = size;
	S->top = 0;
	return YES;
}

Status Pop(Stack* S, PLink* p)//��ջ
{
	if (S->top == 0) { return OVERFLOW; }
	*p = S->person[--S->top];
	return YES;
}

Status Push(Stack* S, People* p)//push
{
	if (S->top == S->size)return OVERFLOW;
	S->person[S->top++] = p;
	return YES;
}

Status IsEmpty_S(Stack* S)//ջ�Ƿ�Ϊ��
{
	if (S->top == 0)return YES;
	return NO;
}


void PrintStack(Stack S)//��ӡջ��Ԫ��
{
	printf("(");
	for (int i = 0; i < S.top; ++i) {
		printf(" %d ", S.person[i]->id);
	}
	printf(")");
}

void PrintTime()//��ӡʱ��
{
	printf("ʱ�䣺%d\n", CurrentTime);
}

int AddPeople()//�������
{
	People* p = (People*)malloc(sizeof(People));
	int af;
	if (!p)return OVERFLOW;
	p->id = people_id;
	p->GiveupTime = rand() % 500 + 100 + CurrentTime;
	p->InterTime = rand() % 250 + 50;
	p->InFloor = rand() % MAXFLOOR;
	p->OutFloor = (p->InFloor + 1 + rand() % (MAXFLOOR - 1)) % MAXFLOOR;
	af = 2 * p->InFloor + (p->InFloor < p->OutFloor);
	QueuePush(&AllFloor[af], p);
	if (af % 2)e.CallUp[p->InFloor] = true;
	else e.CallDown[p->InFloor] = true;
	if (e.stay == true)
	{
		e.CallCar[1] = false;
		e.stay = false;
	}
	printf("�˿�%d������%d�������ǰ��taҪȥ%d�㣬", p->id, p->InFloor, p->OutFloor);
	PrintTime();
	return p->InterTime;
}

void DePeople(int i)
{
	People* p = QueueHead(&AllFloor[i]);
	QueuePop(&AllFloor[i]);
	if (QueueEmpty(&AllFloor[i]))
	{
		if (i % 2)e.CallUp[i / 2] = false;
		else e.CallDown[i / 2] = false;
	}
	if (!(e.floor == p->InFloor))
	{
		printf("�˿�%d�뿪��,", p->id);
		PrintTime();
	}
}

void LackPatience(int i)
{
	if (!QueueEmpty(&AllFloor[i]))
	{
		People* p = QueueHead(&AllFloor[i]);
		if (p->GiveupTime <= CurrentTime)
			DePeople(i);
	}
}

void DeAllFloor()
{
	int i = 0;
	for (i = 0; i < MAXFLOOR * 2; i++)
		if (!(i == 2 * e.floor || i == 2 * e.floor + 1))
			LackPatience(i);
}