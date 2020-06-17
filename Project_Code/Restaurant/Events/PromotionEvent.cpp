
#include "PromotionEvent.h"
#include "..\Rest\Restaurant.h"


PromotionEvent::PromotionEvent(int eTime, int oID,  double AM, ORD_TYPE oType):Event(eTime, oID)
{
	pOrdType = oType;
	AddedMoney = AM;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->promevent(OrderID, AddedMoney);
}
