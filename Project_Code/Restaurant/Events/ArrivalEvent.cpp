#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType,double oMoney, int oSize) :Event(eTime, oID)
{
	OrdType = oType;
	OrdMoney = oMoney;
	OrdSize = oSize;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	Order* pOrd = new Order(OrderID, OrdType);
	pOrd->setmoney(OrdMoney);
	pOrd->setsize(OrdSize);
	pOrd->setArrTime(EventTime);
	pRest->AddtoOrderQueue(pOrd);
}
