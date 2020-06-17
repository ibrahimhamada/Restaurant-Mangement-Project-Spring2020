#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int breakdue;   //Duration of the break of the cook
	int n;			//Break after n orders
	int total_served_orders;       
	int available_time;
	bool assigned;	//states if the cook is assigned or not
	int assigned_to; //order id that he is assigned to, if he/she is not assigned it will be zero
	int BStart;      // the time step which the cook took the break on
	bool IsInBreak; // check if the cook is in a break or not
public:
	Cook();
	virtual ~Cook();
	int n_finished = 0; //number of dishes that the cook has done 
	//Setters and Getters
	int GetID() const;
	ORD_TYPE GetType() const;
	int getSpd() const;
	int getBreakDue() const;
	int getNOrders() const;
	bool isAssigned() const;
	bool GetInBreak() const;
	void SetInBreak(bool);
	void SetBstarts(int);
	int GetBstarts() const;
	int GetAssigned_to() const;
	int GetFinished() const;
	void SetFinished(int);
	void setAssigned_to(int ID);
	void setID(int);
	void setType(ORD_TYPE);
	void setSpd(int);
	void setBreakDue(int);
	void setNorders(int);
	void setAssigned(bool);
	bool isAvailable() const;

	void setAvailabletime(int);
	int getAvailabletime();


};
