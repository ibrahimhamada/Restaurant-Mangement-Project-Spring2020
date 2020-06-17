#ifndef __PROMOTION_EVENT_H_
#define __PROMOTION_EVENT_H_
#include "Event.h"
#include "../Rest/Order.h"


class PromotionEvent : public Event
{
	//info about the order related to Promotion event
	ORD_TYPE pOrdType;		//order type: Normal, vegan, VIP	                
	double AddedMoney;	//Total order money
public:
	PromotionEvent(int eTime, int oID, double AM,ORD_TYPE oType = TYPE_NRM);

	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif