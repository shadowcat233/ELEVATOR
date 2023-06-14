#include"elevator.h"

Condition Close(void)
{
	if (e.InOut)
	{
		AddActivity(CurrentTime + TEST_TIME, Close);
		return Opened;
	}
	//if (IsNeedOpen())
	//{
	//	e.count = 2;
	//	return Opened;
	//}
	e.count = OC_TIME;
	e.IsClosing = true;
	return Closing;
}

Condition In(void)
{
	ElevatorIn(2 * e.floor + (e.state == GoingUp));
	serve_people++;
	if (!QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)]))
		AddActivity(CurrentTime + INOUT_TIME, In);
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
		AddActivity(CurrentTime + INOUT_TIME, Out);
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
	//if (e.floor==0||up &&!down&& e.state == GoingDown)e.state = GoingUp;
	//if (e.floor==MAXFLOOR-1||down && !up&&e.state == GoingUp)e.state = GoingDown;
	//if (e.condition == Opened)e.state = e.CallUp[e.floor];
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
		//e.CallCar[e.floor] = false; 
		//e.state = e.CallUp[e.floor];
		//if (e.state == GoingDown)e.CallDown[e.floor] = false;
		//if (e.state == GoingUp)e.CallUp[e.floor] = false;
		e.InOut = true;
		e.valid_idle = true;
		//if (e.count==OC_TIME-1)
			//AddActivity(CurrentTime + START_CHECK_TIME, Close);
		//if (!e.count)
		//{
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
		//}
		break;
	case Opened:
		//outtime = OC_TIME;
		if (/*e.count &&*/ !IsEmpty_S(e.Stack[e.floor]))
		{
			AddActivity(CurrentTime + INOUT_TIME, Out);
			e.InOut = true;
			break;
		}
		else if (/*e.count && */ !QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)]))
		{
			//if (e.count != 1)outtime = 0;
			AddActivity(CurrentTime + INOUT_TIME, In);
			e.InOut = true;
			break;
		}
		else if (QueueEmpty(&AllFloor[2 * e.floor + (e.state == GoingUp)]))e.InOut = false;;
		//if(IsEmpty_S(e.Stack[e.floor]))e.CallCar[e.floor] = false;
		break;
	case Closing:
		//e.IsClosing = true;
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
		if (!e.count)
		{
			e.count = e.state ? UP_TIME : DOWN_TIME;
			return Moving;
		}
		break;
	case Moving:
		if (e.state == GoingUp && !e.count)
		{
			GoUpOneFloor();
			if (IsNeedStop())
			{
				e.count = UP_DEC_TIME;
				return Decelerate;
			}
			e.count = UP_TIME;
		}
		else if (e.state == GoingDown && !e.count)
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
		if (e.state == GoingUp /* && !e.count */)
			return Arrive;
		else if (e.state == GoingDown/* && !e.count*/)
			return Arrive;
	default:break;
	}
	return e.condition;
}

void AddActivity(int startTime, Condition(*fn)(void))
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

void Act()
{
	if (activity && CurrentTime == activity->startTime)
	{
		Activity* temp = activity;
		e.condition = activity->fn();
		activity = activity->next;
		free(temp);
		Act();
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
	//Activity* act;
	//Condition next_condition;

	while (CurrentTime < 30000)
	{
		if (CurrentTime == people_time)
		{
			people_time += AddPeople();
			people_id++;
		}
		DeAllFloor();
		//e.state = ChangeDirection();
		//CallOutside();
		//CallInside();
		if (!e.count || e.condition == Closed || e.condition == Closing)
		{
			e.state = ChangeDirection();
			e.condition = NextCondition();

		}
		/*if (next_condition != e.condition)
		{
			printf("电梯状态为%s，",ConditionToString(next_condition));
			PrintTime();
			e.condition = next_condition;
		}*/
		Act();
		//act = activity;
		//printf("电梯位于%d层，状态:%d\n",e.floor,e.condition);
		//if (act)printf("下一次活动时间：%d\n",act->startTime);
		if (e.count >= 0)e.count--;
		CurrentTime++;
		//Sleep(100);
	}
	printf("一共来了%d位乘客，有%d位曾乘过电梯。\n", people_id - 1, serve_people);
}