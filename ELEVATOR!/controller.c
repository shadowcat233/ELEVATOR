#include"elevator.h"

Condition Close(void)
{
	if (e.InOut)
	{
		AddActivity(CurrentTime + TEST_TIME, Close);
		return Opened;
	}
	e.count = OC_TIME;
	e.IsClosing = true;
	return Closing;
}

Condition In(void)
{
	ElevatorIn(2 * e.floor + (e.state == GoingUp));
	serve_people++;
	if (!QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)])&&e.peopleNum<MAX_PEOPLE)
		AddActivity(CurrentTime + INOUT_TIME+1, In);
	else
	{
		e.InOut = false;
		e.NoOneInOut = true;
	}
	return Opened;
}
Condition Out(void)
{
	ElevatorOut();
	if (!IsEmpty_S(e.Stack[e.floor]))
	{
		AddActivity(CurrentTime + INOUT_TIME+1, Out);
		return Opened;
	}
	e.CallCar[e.floor] = false;
	e.count = 1;
	return Opened;
}

Condition NextCondition()
{
	int up, down, open;
	up = IsNeedUp(); down = IsNeedDown(); open = IsNeedOpen();
	switch (e.condition)
	{
	case Arrive:
		if (open)
		{
			e.count = OC_TIME;
			AddActivity(CurrentTime + START_CHECK_TIME + 1, Close);
			return Opening;
		}
		if (up || down)
		{
			e.count = ACCELERATE_TIME;
			return Accelerate;
		}
		e.CallCar[1] = false;
		e.count = DELAY_TIME;
		return Closed;
		break;
	case Opening:
		e.InOut = true;
		e.valid_idle = true;

		if (e.IsClosing == true)
		{
			e.IsClosing = false;
			e.count = OC_TIME;
			AddActivity(CurrentTime + START_CHECK_TIME, Close);
			return Opening;
		}
		e.count = 1;
		printf("电梯门已打开，");
		PrintTime();
		return Opened;
		break;
	case Opened:
		if (!IsEmpty_S(e.Stack[e.floor]))
		{
			AddActivity(CurrentTime + INOUT_TIME, Out);
			e.InOut = true;
			break;
		}
		else if (!QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)])&&e.peopleNum<MAX_PEOPLE)
		{
			AddActivity(CurrentTime + INOUT_TIME, In);
			e.InOut = true;
			break;
		}
		else if (QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)]))e.InOut = false;;
		break;
	case Closing:
		if (open)
		{
			e.count = CANCLE_CLOSE_TIME;
			e.IsClosing = true;
			return Opening;
		}
		if (!e.count)
		{
			e.count = DELAY_TIME;
			e.NoOneInOut = false;
			printf("电梯门已关闭，");
			PrintTime();
			e.count = 1;
			return Closed;
		}
		break;
	case Closed:
		e.IsClosing = false;
		if (open)
		{
			e.count = 1;
			return Arrive;
		}
		if (up || down)
		{
			e.count = ACCELERATE_TIME;
			return Accelerate;
		}
		if (!e.count && e.floor != 1)
		{
			e.CallCar[1] = true;
			e.stay = true;
		}
		break;
	case Accelerate:
			e.count = e.state ? UP_TIME : DOWN_TIME;
			return Moving;
	case Moving:
		if (e.state == GoingUp)
		{
			GoUpOneFloor();
			if (IsNeedStop())
			{
				e.count = UP_DEC_TIME;
				return Decelerate;
			}
			e.count = UP_TIME;
		}
		else
		{
			GoDownOneFloor();
			if (IsNeedStop())
			{
				e.count = DOWN_DEC_TIME;
				return Decelerate;
			}
			e.count = DOWN_TIME;
		}
		break;
	case Decelerate:
		e.count = 1;
		return Arrive;
	default:break;
	}
	return e.condition;
}

void AddActivity(int startTime, Condition(*fn)(void))//添加活动
{
	Activity* temp = (Activity*)malloc(sizeof(Activity));
	if (!temp)exit(-1);
	temp->startTime = startTime;
	temp->fn = fn;
	temp->next = NULL;
	if (!activity)
		activity = temp;
	else
	{
		if (!activity->next && temp->startTime < activity->startTime)
		{
			temp->next = activity;
			activity = temp;
		}
		else
		{
			Activity* p = activity;
			while (p->next && p->next->startTime < startTime)
				p = p->next;
			temp->next = p->next;
			p->next = temp;
		}
	}
}

void Act()//到达开始时间时进行活动
{
	if (activity && CurrentTime == activity->startTime)
	{
		Activity* temp = activity;
		e.condition = activity->fn();
		activity = activity->next;
		free(temp);
		Act();//以防同一时刻有多个活动开始
	}
}

void init()
{
	AllQueueInit();
	InitElevator();
	activity = NULL;
	srand((unsigned)time(NULL));
}

char* ConditionToString(Condition con)
{
	switch (con)
	{
	case Arrive:return "ARRIVE";
	case Opening:return "OPENING";
	case Opened:return "OPENED";
	case Closing:return "CLOSING";
	case Closed:return "CLOSED";
	case Moving:return "MOVING";
	case Accelerate:return "ACCELERATE";
	case Decelerate:return "DECELERATE";
	default:return "UNKNOW";
	}
}

void run()
{
	CurrentTime = 0;
	people_id = 1;
	serve_people = 0;
	e.count = 0;
	int people_time = 0;
	bool add = true;

	while (CurrentTime < MAX_RUN_TIME)
	{
		if (CurrentTime == people_time)
		{
			people_time += AddPeople();
			people_id++;
			add = true;
		}
		DeAllFloor();//耐心为0的人离开
		if (!e.count || add&&(e.condition == Closed || e.condition == Closing))
		{//倒计时为0或在CLOSED或closing状态且正好来了一个人时需要判断电梯接下来的状态
			e.state = ChangeDirection();
			e.condition = NextCondition();
			add = false;
		}
		Act();
		if (e.count >= 0)e.count--;
		CurrentTime++;
		Sleep(10);//每0.01秒执行一次，相当于加速十倍
	}
	printf("一共来了%d位乘客，有%d位曾乘过电梯。\n", people_id - 1, serve_people);
 }