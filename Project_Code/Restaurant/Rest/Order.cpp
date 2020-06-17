#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0 && id<1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetDistance(int d)
{
	Distance = d>0 ? d : 0;
}

int Order::GetDistance() const
{
	return Distance;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setpriority(int p)
{
	Priority = p;
}

int Order::getpriority()
{
	return Priority;
}

void Order::setsize(int s)
{
	size = s;
}
int Order::getsize()
{
	return size;
}
void Order::setArrTime(int arr)
{
	ArrTime = arr;
}
void Order::setservtime(int serv)
{
	ServTime = serv;
}
void Order::setfinishtime(int finish)
{
	FinishTime = finish;
}
void Order::setmoney(double m)
{
	totalMoney = m;
}
int Order::getArrTime()
{
	return ArrTime;
}
int Order::getservtime()
{
	return ServTime;
}
int Order::getfinishtime()
{
	return FinishTime;
}
double Order::getmoney()
{
	return totalMoney;
}
void Order::setautoProm(bool n)
{
	autoProm = n;
}
bool Order::getauroProm() const
{
	return autoProm;
}
void Order::setType(ORD_TYPE T)
{
	type = T;
}