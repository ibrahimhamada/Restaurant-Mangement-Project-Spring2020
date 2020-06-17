#include "Cook.h"


Cook::Cook()
{
	assigned = false;
	IsInBreak = false;
	total_served_orders = 0;
	available_time = 0;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

int Cook::getSpd() const
{
	return speed;
}

int Cook::getBreakDue() const
{
	return breakdue;
}

int Cook::getNOrders() const
{
	return n;
}

bool Cook::isAssigned() const
{
	return assigned;
}


bool Cook::GetInBreak() const
{
	return IsInBreak;
}

void Cook::SetInBreak(bool b)
{
	IsInBreak = b;
}

void Cook::SetBstarts(int bs)
{
	BStart = bs;
}

int Cook::GetBstarts() const
{
	return BStart;
}



int Cook::GetAssigned_to() const
{
	if (assigned)
		return assigned_to;
	else return -1;
}

int Cook::GetFinished() const
{
	return n_finished;
}

void Cook::SetFinished(int f)
{
	n_finished = f;
}

void Cook::setAssigned_to(int ID)
{
	if (assigned) {
		assigned_to = ID;
		n_finished++;
		total_served_orders++;
	}
}


void Cook::setSpd(int s)
{
	speed = s;
}

void Cook::setBreakDue(int b)
{
	breakdue = b;
}

void Cook::setNorders(int orders)
{
	n = orders;
}

void Cook::setAssigned(bool assign)
{
	assigned = assign;
}

bool Cook::isAvailable() const
{
	if (!assigned && !IsInBreak) {
		return true;
	}
	else
		return false;
}


void Cook::setAvailabletime(int n) {
	available_time = n;
}

int Cook::getAvailabletime() {
	return available_time;
}