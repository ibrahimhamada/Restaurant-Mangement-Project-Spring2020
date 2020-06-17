#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant

	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	int size;   //size of the order
	int Priority;  //determines the priority of the order

	bool autoProm = false;
public:
	Order(int ID, ORD_TYPE r_Type);
	virtual ~Order();
// Setters and Getters
	int GetID();
	ORD_TYPE GetType() const;
	void SetDistance(int d);
	int GetDistance() const;
	void setType(ORD_TYPE);
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	void setpriority(int);
	int getpriority();
	void setsize(int);
	int getsize();
	void setArrTime(int);
	void setservtime(int);
	void setfinishtime(int);
	void setmoney(double);
	int getArrTime();
	int getservtime();
	int getfinishtime();
	double getmoney();
	void setautoProm(bool);
	bool getauroProm() const;

};

#endif