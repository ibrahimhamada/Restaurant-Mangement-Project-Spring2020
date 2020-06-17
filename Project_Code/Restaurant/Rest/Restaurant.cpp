#include <cstdlib>
#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancelEvent.h"
#include "..\Events\PromotionEvent.h"





Restaurant::Restaurant()
{
	pGUI = NULL;
}

//This function calls the three simulation modes' function.
void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	
	{
	case MODE_INTR:
		Interactive_simulation();
		break;
	case MODE_STEP:
		StepByStep_simulation();
		break;
	case MODE_SLNT:
		Silent_simulation();
		break;
	default: 
		break;
	};

}

//This function returns a list of availble cooks who are not neither serving any order nor taking a break.
Cook* Restaurant::AvailableCooks()
{
	int* c = this->getAvailableCook();
	int count = c[0] + c[1] + c[2];
	Cook* pC = new Cook[count];
	Cook* FirstCook = pC;
	int j = 0;
	for (int i = 0; i < NC_Count || j < c[0]; i++)
		if (!NCooks[i].isAssigned() && !NCooks[i].GetInBreak())
		{
			pC[j] = NCooks[i];
			j++;
		}
	for (int i = 0; i < GC_Count || j < c[0] + c[1]; i++)
		if (!GCooks[i].isAssigned() && !GCooks[i].GetInBreak())
		{
			pC[j] = GCooks[i];
			j++;
		}
	for (int i = 0; i < VC_Count || j < count; i++)
		if (!VCooks[i].isAssigned() && !VCooks[i].GetInBreak())
		{
			pC[j] = VCooks[i];
			j++;
		}
	return FirstCook;
}



//This function is used to cancel a normal order which is still in the waiting orders and not served yet.
void Restaurant::CancelOrder(int id)
{
	int c;
	Order** orders = Wait_Norm_Orders.toArray(c);
	for (int i = 0; i < c; i++)
		if (orders[i]->GetID() == id)
		{
			Wait_Norm_Orders.DeleteNode(orders[i]);
			return;
		}
}

//This function returns the number of availble cooks who are not neither serving any order nor taking a break.
int* Restaurant::getAvailableCook() const
{
	int* cooks = new int[3];
	int c = 0;
	for (int i = 0; i < NC_Count; i++) {
		if (!NCooks[i].isAssigned() && !NCooks[i].GetInBreak())
			c++;
	}
	cooks[0] = c;
	c = 0;
	for (int i = 0; i < GC_Count; i++) {
		if (!GCooks[i].isAssigned() && !GCooks[i].GetInBreak())
			c++;
	}
	cooks[1] = c;
	c = 0;
	for (int i = 0; i < VC_Count; i++) {
		if (!VCooks[i].isAssigned() && !VCooks[i].GetInBreak())
			c++;
	}
	cooks[2] = c;
	return cooks;
}




//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}



//This function adds each order to its list base on its type.
void Restaurant::AddtoOrderQueue(Order* pOrd)
{
	if (pOrd->GetType() == TYPE_NRM)
	{
		Wait_Norm_Orders.InsertEnd(pOrd);
	}
	if (pOrd->GetType() == TYPE_VGAN)
	{
		Wait_Veg_Orders.enqueue(pOrd);
	}
	if (pOrd->GetType() == TYPE_VIP)
	{
		Wait_VIP_Orders.enqueue(pOrd,pOrd->getArrTime()*pOrd->getmoney()/pOrd->getsize());
	}
}

//This function excute the promotion event.
void Restaurant::promevent(int id, double money)
{
	int c;
	Order** orders = Wait_Norm_Orders.toArray(c);
	for (int i = 0; i < c; i++)
		if (orders[i]->GetID() == id)
		{
			Wait_Norm_Orders.DeleteNode(orders[i]);
			orders[i]->setmoney(money + orders[i]->getmoney());
			orders[i]->setType(TYPE_VIP);
			Wait_VIP_Orders.enqueue(orders[i], orders[i]->getArrTime()*orders[i]->getmoney() / orders[i]->getsize());
			return;
		}
}


//This function excute auto promotion event.
int Restaurant::autoprom(int t)
{
	int k, c = 0;
	Order** NwtOrds = Wait_Norm_Orders.toArray(k);
	for (int i = 0; i < k; i++)
		if (t - NwtOrds[i]->getArrTime() == auto_p)
		{
			promevent(NwtOrds[i]->GetID(), 0);
			c++;
		}
	return c;
}

//This function returns the number of Normal waiting orders.
int Restaurant::getNormWait()
{
	return Wait_Norm_Orders.getCount();
}

//This function returns the number of VIP waiting orders.
int Restaurant::getVIPWait()
{
	return Wait_VIP_Orders.getCount();
}

//This function returns the number of Vegan waiting orders.
int Restaurant::getVegWait()
{
	Order* temp;
	Queue<Order*> tempQueue;
	int c = 0;
	while (Wait_Veg_Orders.dequeue(temp))
	{
		c++;
		tempQueue.enqueue(temp);
	}
	while (tempQueue.dequeue(temp))
		Wait_Veg_Orders.enqueue(temp);
	return c;
}


//This function is used to draw all items [cooks and orders] in GUI in the same order required in the project doc.
void Restaurant::FillDrawingList()
{
	int* C_count = this->getAvailableCook();
	int count = C_count[0] + C_count[1] + C_count[2];
	Cook* pC = this->AvailableCooks();
	
	//Add ALL Cooks to GUI sorted based on the available time
	//Here, we assumed that available time is the time at which the cook finished serving a certain order or returns from his break.
	for (int i = 0; i < count; i++) {
		for (int j = i + 1; j < count; j++) {
			if (pC[j].getAvailabletime() < pC[i].getAvailabletime()) {
				Cook tempC;
				tempC = pC[i];
				pC[i] = pC[j];
				pC[j] = tempC;
			}
		}
	}
	
	for (int i = 0; i < count; i++) {
		pGUI->AddToDrawingList(&pC[i]);
	}
	///////////////////////////////////////////////////////////////////

	//Add ALL orders to GUI.

	//Add ALL Waiting orders sorted based on their Arrival time
	int sizeNW = 0, sizeVW = 0, sizeGW = 0;
	Order** NW_Orders = Wait_Norm_Orders.toArray(sizeNW);
	Order** GWOrders = Wait_Veg_Orders.toArray(sizeGW);
	Order** VWOrders = Wait_VIP_Orders.toArray(sizeVW);

	int sizeW = 0;
	LinkedList<Order*> W_Orders;

	Order* pOrd;

	for (int i = 0; i < sizeNW; i++)
	{
		W_Orders.InsertEnd(NW_Orders[i]);
	}
	for (int i = 0; i < sizeGW; i++)
	{
		W_Orders.InsertEnd(GWOrders[i]);
	}
	for (int i = 0; i < sizeVW; i++)
	{
		W_Orders.InsertEnd(VWOrders[i]);
	}

	Order** Wait_Orders = W_Orders.toArray(sizeW);
	for (int i = 0; i < sizeW; i++) {
		for (int j = i + 1; j < sizeW; j++) {
			if (Wait_Orders[j]->getArrTime() < Wait_Orders[i]->getArrTime()) {
				Order* tempO;
				tempO = Wait_Orders[i];
				Wait_Orders[i] = Wait_Orders[j];
				Wait_Orders[j] = tempO;
			}
		}
	}

	// Add Waiting Orders ordered by arrival time 
	for (int i = 0; i < sizeW; i++)
	{
		pOrd = Wait_Orders[i];
		pGUI->AddToDrawingList(pOrd);
	}
	//////////////////////////////////////////////////////////////////////


    //Add ALL In Service orders sorted based on their Service time
	int sizeNIn = 0, sizeVIn = 0, sizeGIn = 0;
	Order** NIn_Orders = InServ_Norm_Orders.toArray(sizeNIn);
	Order** GInOrders = InServ_Veg_Orders.toArray(sizeGIn);
	Order** VInOrders = InServ_VIP_Orders.toArray(sizeVIn);

	int sizeServ = 0;
	LinkedList<Order*> InServ_Orders;

	for (int i = 0; i < sizeNIn; i++)
	{
		InServ_Orders.InsertEnd(NIn_Orders[i]);
	}
	for (int i = 0; i < sizeGIn; i++)
	{
		InServ_Orders.InsertEnd(GInOrders[i]);
	}
	for (int i = 0; i < sizeVIn; i++)
	{
		InServ_Orders.InsertEnd(VInOrders[i]);
	}

	Order** In_Orders = InServ_Orders.toArray(sizeServ);
	for (int i = 0; i < sizeServ; i++) {
		for (int j = i + 1; j < sizeServ; j++) {
			if (In_Orders[j]->getservtime() < In_Orders[i]->getservtime()) {
				Order* tempO;
				tempO = In_Orders[i];
				In_Orders[i] = In_Orders[j];
				In_Orders[j] = tempO;
			}
		}
	}

	for (int i = 0; i < sizeServ; i++)
	{
		pOrd = In_Orders[i];
		pGUI->AddToDrawingList(pOrd);
	}
	////////////////////////////////////////////////////////////////////////////////

	//Add ALL Finished orders sorted based on their Finished time
	int sizeF = 0;
	Order** F_Orders = Finished_Orders.toArray(sizeF);
	for (int i = 0; i < sizeF; i++)
	{
		pOrd = F_Orders[i];
		pGUI->AddToDrawingList(pOrd);
	}


}


//This function is used to load the text file that contains the events which will be excuted.
void Restaurant::LoadFile(string f)
{
	int No_events; // number of events and auto pormotion
	char event_type;
	Event* pEv;
	ifstream input_file;
	input_file.open(f, ios::in);
	while (!(input_file)) {
		pGUI->PrintMessage ("File Not Found! Please Enter a valid file name: " );
		f = pGUI->GetString().c_str();
		input_file.open(f, ios::in);
	}

	if (input_file) {
		input_file >> NC_Count >> GC_Count >> VC_Count;
		input_file >> SN >> SG >> SV;
		input_file >> BO >> BN >> BG >> BV;
		input_file >> auto_p;
		input_file >> No_events;
		for (int i = 0; i < No_events; i++)
		{
			input_file >> event_type;
			if (event_type == 'R')
			{
				int TS, ID, SIZE, MONY;
				char OType;
				input_file >> OType >> TS >> ID >> SIZE >> MONY;
				switch (OType)
				{
				case 'N':
					pEv = new ArrivalEvent(TS, ID, TYPE_NRM, MONY, SIZE);
					break;
				case 'G':
					pEv = new ArrivalEvent(TS, ID, TYPE_VGAN, MONY, SIZE);
					break;
				case 'V':
					pEv = new ArrivalEvent(TS, ID, TYPE_VIP, MONY, SIZE);
					break;
				}
				EventsQueue.enqueue(pEv);
			}
			if (event_type == 'X')
			{
				int TS, ID;
				input_file >> TS >> ID;
				pEv = new CancelEvent(TS, ID);
				EventsQueue.enqueue(pEv);
			}

			if (event_type == 'P')
			{
				int TS, ID, ExMony;
				input_file >> TS >> ID >> ExMony;
				pEv = new PromotionEvent(TS, ID, ExMony);
				EventsQueue.enqueue(pEv);
			}

		}
		input_file.close();
	}
}

//This function will generate the required OUTPUT TEXT file.
void Restaurant::OutputFile(int aProms)
{
	Order* order;
	Order* order2;
	ofstream f;
	f.open("Output.txt", ios::out);
	if (!f)
	{
		pGUI->PrintMessage("Error: Output File Corrupted!");
		return;
	}
	int c = 0, Nc = 0, Gc = 0, Vc = 0, sumWT = 0, sumST = 0;
	f << "FT   ID   AT   WT   ST" << endl;
	Queue<Order*>temp;

	while (Finished_Orders.dequeue(order))
	{
		Finished_Orders.peekFront(order2);
		if (order->getfinishtime() == order2->getfinishtime())
		{
			if (order->getfinishtime() - order->getservtime() > order2->getfinishtime() - order2->getservtime())
			{
				Finished_Orders.dequeue(order2);
				temp.enqueue(order2);
				temp.enqueue(order);
				continue;
			}
		}
		temp.enqueue(order);
	}

	while (temp.dequeue(order))
		Finished_Orders.enqueue(order);

	while (Finished_Orders.dequeue(order))
	{
		f << order->getfinishtime() << "    " << order->GetID() << "    " << order->getArrTime() << "    " << order->getservtime() - order->getArrTime() << "    " << order->getfinishtime() - order->getservtime() << endl;
		if (order->GetType() == TYPE_NRM)
			Nc++;
		else if (order->GetType() == TYPE_VGAN)
			Gc++;
		else if (order->GetType() == TYPE_VIP)
			Vc++;
		c++;
		sumWT += order->getservtime() - order->getArrTime();
		sumST += order->getfinishtime() - order->getservtime();
	}


	f << "Orders: " << c << " [Norm: " << Nc << ", Veg: " << Gc << ", VIP: " << Vc << "]" << endl;
	f << "Cooks: " << VC_Count + GC_Count + NC_Count << " [Norm: " << NC_Count << ", Veg: " << GC_Count << ", VIP: " << VC_Count << "]" << endl;
	f << fixed << setprecision(2) << "Avg Wait = " << (double)sumWT / (double)c << ", Avg Serv = " << (double)sumST / (double)c << endl;
	f << "Auto - promoted: " << aProms;
}


void Restaurant::UpdateStatusbar(int t)
{
	int AvN = getNormWait();
	int AvG = getVegWait();
	int AvV = getVIPWait();
	int* Av = getAvailableCook();
	int AvCN = Av[0];
	int AvCG = Av[1];
	int AvCV = Av[2];
	pGUI->PrintAvailable(AvN, AvG, AvV, AvCN, AvCG, AvCV, t, msgs, amsg, servN, servG, servV);
}


void Restaurant::SetCookFree(int Order_ID, ORD_TYPE type)
{
	switch (type)
	{
	case TYPE_NRM:
		for (int i = 0; i < NC_Count; i++) {
			if (NCooks[i].isAssigned() && NCooks[i].GetAssigned_to() == Order_ID)
				NCooks[i].setAssigned(false);
		}
		break;
	case TYPE_VGAN:
		for (int i = 0; i < GC_Count; i++) {
			if (GCooks[i].isAssigned() && GCooks[i].GetAssigned_to() == Order_ID)
				GCooks[i].setAssigned(false);
		}
		break;
	case TYPE_VIP:
		for (int i = 0; i < VC_Count; i++) {
			if (VCooks[i].isAssigned() && VCooks[i].GetAssigned_to() == Order_ID)
				VCooks[i].setAssigned(false);
		}
		break;
	}
}


//The following functions are used to assign the orders to cooks based on the required criteria described in the project doc.
//////////////////////////////////////////////////////////////////////////////////////////

//This function is used to assign the VIP Orders in the waiting Class to VIP Cooks, 
//then Vegan Cook if no VIP Cook is found, then Normal Cooks if no VIP and Vegan Cooks are found.
void Restaurant::VIP2Service(int CurrentTimeStep) {
	int sizeWV = 0;
	int d;
	Order** VWait_Array;
	Order * pWVOrd;   //pointer to Wait VIP orders
	VWait_Array = Wait_VIP_Orders.toArray(sizeWV);

	for (int i = 0; (i < sizeWV); i++)
	{
		d = 0;
		pWVOrd = VWait_Array[i];
		Order* pIVOrd = pWVOrd;   //pointer to In Service VIP orders
		for (int j = 0; j < VC_Count; j++) {
			if (VCooks[j].isAvailable()) {
				VCooks[j].setAssigned(1);
				VCooks[j].setAssigned_to(VWait_Array[i]->GetID());
				InServ_VIP_Orders.InsertEnd(pIVOrd);
				Wait_VIP_Orders.dequeue(pWVOrd);
				pIVOrd->setservtime(CurrentTimeStep);
				pIVOrd->setStatus(SRV);
				d = 1;
				msgs[amsg] = "V" + to_string(VCooks[j].GetID()) + "(V" + to_string(VWait_Array[i]->GetID()) + ") ";
				amsg++;
				break;
			}
		}
		if (d != 1) {
			for (int j = 0; j < GC_Count; j++) {
				if (GCooks[j].isAvailable()) {
					GCooks[j].setAssigned(1);
					GCooks[j].setAssigned_to(VWait_Array[i]->GetID());
					InServ_VIP_Orders.InsertEnd(pIVOrd);
					Wait_VIP_Orders.dequeue(pWVOrd);
					pIVOrd->setservtime(CurrentTimeStep);
					pIVOrd->setStatus(SRV);
					d = 1;
					msgs[amsg] = "G" + to_string(GCooks[j].GetID()) + "(V" + to_string(VWait_Array[i]->GetID()) + ") ";
					amsg++;
					break;
				}
			}
		}
		if (d != 1) {
			for (int j = 0; j < NC_Count; j++) {
				if (NCooks[j].isAvailable()) {
					NCooks[j].setAssigned(1);
					NCooks[j].setAssigned_to(VWait_Array[i]->GetID());
					InServ_VIP_Orders.InsertEnd(pIVOrd);
					Wait_VIP_Orders.dequeue(pWVOrd);
					pIVOrd->setservtime(CurrentTimeStep);
					pIVOrd->setStatus(SRV);
					d = 1;
					msgs[amsg] = "N" + to_string(NCooks[j].GetID()) + "(V" + to_string(VWait_Array[i]->GetID()) + ") ";
					amsg++;
					break;
				}
			}
		}

	}
}


//This function is used to assign the Vegan Orders in the waiting Class to Vegan Cooks Only. 
void Restaurant::Vegan2Service(int CurrentTimeStep) {

	int sizeWG = 0;
	Order** GWait_Array;
	Order* pWGOrd;  //pointer to Wait Vegan orders
	GWait_Array = Wait_Veg_Orders.toArray(sizeWG);

	for (int i = 0; (i < sizeWG); i++) {
		pWGOrd = GWait_Array[i];
		Order* pIGOrd = pWGOrd;   //pointer to In Service Vegan orders

		for (int j = 0; j < GC_Count; j++) {
			if (GCooks[j].isAvailable()) {
				GCooks[j].setAssigned(1);
				GCooks[j].setAssigned_to(GWait_Array[i]->GetID());
				InServ_Veg_Orders.enqueue(pIGOrd);
				Wait_Veg_Orders.dequeue(pWGOrd);
				pWGOrd->setservtime(CurrentTimeStep);
				pWGOrd->setStatus(SRV);
				msgs[amsg] = "G" + to_string(GCooks[j].GetID()) + "(G" + to_string(GWait_Array[i]->GetID()) + ") ";
				amsg++;
				break;
			}
		}
	}
}

//This function is used to assign the Normal Orders in the waiting Class to Normal Cooks, 
//then VIP Cook if no Normal Cook is found.
void Restaurant::Normal2Service(int CurrentTimeStep) {
	int sizeWN = 0;
	int d;
	Order** NWait_Array;
	Order* pWNOrd;  //pointer to Wait Normal orders
	NWait_Array = Wait_Norm_Orders.toArray(sizeWN);

	for (int i = 0; (i < sizeWN); i++) {
		d = 0;
		pWNOrd = NWait_Array[i];
		Order* pINOrd = pWNOrd;   //pointer to In Service Normal orders
		for (int j = 0; j < NC_Count; j++) {
			if (NCooks[j].isAvailable()) {
				NCooks[j].setAssigned(1);
				NCooks[j].setAssigned_to(NWait_Array[i]->GetID());
				InServ_Norm_Orders.InsertEnd(pINOrd);
				Wait_Norm_Orders.DeleteNode(pWNOrd);
				pINOrd->setservtime(CurrentTimeStep);
				pINOrd->setStatus(SRV);
				d = 1;
				msgs[amsg] = "N" + to_string(NCooks[j].GetID()) + "(N" + to_string(NWait_Array[i]->GetID()) + ") ";
				amsg++;
				break;
			}
		}
		if (d != 1) {
			for (int j = 0; j < VC_Count; j++) {
				if (VCooks[j].isAvailable()) {
					VCooks[j].setAssigned(1);
					VCooks[j].setAssigned_to(NWait_Array[i]->GetID());
					InServ_Norm_Orders.InsertEnd(pINOrd);
					Wait_Norm_Orders.DeleteNode(pWNOrd);
					pINOrd->setservtime(CurrentTimeStep);
					pINOrd->setStatus(SRV);
					d = 1;
					msgs[amsg] = "V" + to_string(VCooks[j].GetID()) + "(N" + to_string(NWait_Array[i]->GetID()) + ") ";
					amsg++;
					break;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

//The following functions are used to determine which orders are to be finished in the 
//current timestep based on the order's dishes and the speed of cook serving this order.

//This function is used to determine which VIP Orders in the In Service Class are finished. 
void Restaurant::VIP2Finished(int CurrentTimeStep) {

	int sizeInV = 0;
	int d;
	Order** VIn_Array;
	Order* pInVOrd;   //pointer to In Service VIP orders

	VIn_Array = InServ_VIP_Orders.toArray(sizeInV);
	int s = 0;
	for (int i = 0; (i < sizeInV); i++)
	{
		d = 0;
		pInVOrd = VIn_Array[i];
		for (int j = 0; j < VC_Count; j++) {
			if (VCooks[j].isAssigned() && (VCooks[j].GetAssigned_to() == VIn_Array[i]->GetID())) {
				s = (VIn_Array[i]->getsize() % VCooks[j].getSpd() == 0) ? (VIn_Array[i]->getsize() / VCooks[j].getSpd())  
					: (VIn_Array[i]->getsize() / VCooks[j].getSpd() +1);

				if ((CurrentTimeStep - VIn_Array[i]->getservtime()) == s) {
					Order* pFOrd = pInVOrd;   //pointer to Finished orders
					Finished_Orders.enqueue(pFOrd);
					InServ_VIP_Orders.DeleteNode(pInVOrd);
					pFOrd->setfinishtime(CurrentTimeStep);
					pFOrd->setStatus(DONE);
					VCooks[j].setAssigned(0);
					VCooks[j].setAvailabletime(CurrentTimeStep);
					d = 1;
					servV++;
					break;
				}
			}
		}
		if (d != 1) {
			for (int j = 0; j < GC_Count; j++) {
				if (GCooks[j].isAssigned() && (GCooks[j].GetAssigned_to() == VIn_Array[i]->GetID())) {
					s = (VIn_Array[i]->getsize() % GCooks[j].getSpd() == 0) ? (VIn_Array[i]->getsize() / GCooks[j].getSpd())  
						: (VIn_Array[i]->getsize() / GCooks[j].getSpd() +1);
					if ((CurrentTimeStep - VIn_Array[i]->getservtime()) == s) {
						Order* pFOrd = pInVOrd;   //pointer to Finished orders
						Finished_Orders.enqueue(pFOrd);
						InServ_VIP_Orders.DeleteNode(pInVOrd);
						pFOrd->setfinishtime(CurrentTimeStep);
						pFOrd->setStatus(DONE);
						GCooks[j].setAssigned(0);
						GCooks[j].setAvailabletime(CurrentTimeStep);
						d = 1;
						servV++;
						break;

					}
				}
			}
		}
		if (d != 1) {
			for (int j = 0; j < NC_Count; j++) {
				if (NCooks[j].isAssigned() && (NCooks[j].GetAssigned_to() == VIn_Array[i]->GetID())) {
					s = (VIn_Array[i]->getsize() % NCooks[j].getSpd() == 0) ? (VIn_Array[i]->getsize() / NCooks[j].getSpd())
						: (VIn_Array[i]->getsize() / NCooks[j].getSpd() + 1);
					if ((CurrentTimeStep - VIn_Array[i]->getservtime()) == s) {
						Order* pFOrd = pInVOrd;   //pointer to Finished orders
						Finished_Orders.enqueue(pFOrd);
						InServ_VIP_Orders.DeleteNode(pInVOrd);
						pFOrd->setfinishtime(CurrentTimeStep);
						pFOrd->setStatus(DONE);
						NCooks[j].setAssigned(0);
						NCooks[j].setAvailabletime(CurrentTimeStep);
						d = 1;
						servV++;
						break;
					}
				}
			}
		}
	}
}

//This function is used to determine which Vegan Orders in the In Service Class are finished. 
void Restaurant::Vegan2Finished(int CurrentTimeStep) {

	int sizeInG = 0;
	Order** GIn_Array;
	Order* pInGOrd;   //pointer to In Service Vegan orders

	GIn_Array = InServ_Veg_Orders.toArray(sizeInG);
	int s = 0;
	for (int i = 0; (i < sizeInG); i++)
	{
		pInGOrd = GIn_Array[i];
		for (int j = 0; j < GC_Count; j++) {
			if (GCooks[j].isAssigned() && (GCooks[j].GetAssigned_to() == GIn_Array[i]->GetID())) {
				s = (GIn_Array[i]->getsize() % GCooks[j].getSpd() == 0) ? (GIn_Array[i]->getsize() / GCooks[j].getSpd()) 
					: (GIn_Array[i]->getsize() / GCooks[j].getSpd() +1 );
				if ((CurrentTimeStep - GIn_Array[i]->getservtime()) == s) {
					Order* pFOrd = pInGOrd;   //pointer to Finished orders
					Finished_Orders.enqueue(pFOrd);
					Order* temp;
					Queue<Order*> tempQueue;
					while (!(InServ_Veg_Orders.isEmpty())) {
						InServ_Veg_Orders.dequeue(temp);
						if (temp->GetID() != pFOrd->GetID()) {
							tempQueue.enqueue(temp);
						}
					}
					while (!(tempQueue.isEmpty())) {
						tempQueue.dequeue(temp);
						InServ_Veg_Orders.enqueue(temp);
					}
					pFOrd->setfinishtime(CurrentTimeStep);
					pFOrd->setStatus(DONE);
					GCooks[j].setAssigned(0);
					GCooks[j].setAvailabletime(CurrentTimeStep);
					servG++;
				}
			}
		}

	}
}

//This function is used to determine which Normal Orders in the In Service Class are finished. 
void Restaurant::Normal2Finished(int CurrentTimeStep) {

	int sizeInN = 0;
	int d;
	Order** NIn_Array;
	Order* pInNOrd;   //pointer to In Service Normal orders

	NIn_Array = InServ_Norm_Orders.toArray(sizeInN);
	int s = 0;
	for (int i = 0; (i < sizeInN); i++)
	{
		d = 0;
		pInNOrd = NIn_Array[i];
		for (int j = 0; j < NC_Count; j++) {
			if (NCooks[j].isAssigned() && (NCooks[j].GetAssigned_to() == NIn_Array[i]->GetID())) {
				s = (NIn_Array[i]->getsize() % NCooks[j].getSpd() == 0) ? (NIn_Array[i]->getsize() / NCooks[j].getSpd())  
					: (NIn_Array[i]->getsize() / NCooks[j].getSpd() + 1);
				if ((CurrentTimeStep - NIn_Array[i]->getservtime()) == s) {
					Order* pFOrd = pInNOrd;   //pointer to Finished orders
					Finished_Orders.enqueue(pFOrd);
					InServ_Norm_Orders.DeleteNode(pInNOrd);
					pFOrd->setfinishtime(CurrentTimeStep);
					pFOrd->setStatus(DONE);
					NCooks[j].setAssigned(0);
					NCooks[j].setAvailabletime(CurrentTimeStep);
					d = 1;
					servN++;
					break;
				}
			}
		}
		if (d != 1) {
			for (int j = 0; j < VC_Count; j++) {
				if (VCooks[j].isAssigned() && (VCooks[j].GetAssigned_to() == NIn_Array[i]->GetID())) {
					s = (NIn_Array[i]->getsize() % VCooks[j].getSpd() == 0) ? (NIn_Array[i]->getsize() / VCooks[j].getSpd()) 
						: (NIn_Array[i]->getsize() / VCooks[j].getSpd() + 1 );
					if ((CurrentTimeStep - NIn_Array[i]->getservtime()) == s) {
						Order* pFOrd = pInNOrd;   //pointer to Finished orders
						Finished_Orders.enqueue(pFOrd);
						InServ_Norm_Orders.DeleteNode(pInNOrd);
						pFOrd->setfinishtime(CurrentTimeStep);
						pFOrd->setStatus(DONE);
						VCooks[j].setAssigned(0);
						VCooks[j].setAvailabletime(CurrentTimeStep);
						d = 1;
						servN++;
						break;
					}
				}
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////

//This function determines which cooks should have break if they finished the required orders based on the cook type.
void Restaurant::CooksToBreak(int CurrentTimeStep) {
	for (int j = 0; j < NC_Count; j++) {
		if (NCooks[j].isAvailable()) {
			if (NCooks[j].n_finished != 0 && NCooks[j].n_finished % BO == 0) {
				NCooks[j].SetBstarts(CurrentTimeStep);
				NCooks[j].SetInBreak(1);
			}
		}
	}
	for (int j = 0; j < VC_Count; j++) {
		if (VCooks[j].isAvailable()) {
			if (VCooks[j].n_finished != 0 &&  VCooks[j].n_finished % BO == 0) {
				VCooks[j].SetBstarts(CurrentTimeStep);
				VCooks[j].SetInBreak(1);
			}
		}
	}
	for (int j = 0; j < GC_Count; j++) {
		if (GCooks[j].isAvailable()) {
			if (GCooks[j].n_finished != 0 &&  GCooks[j].n_finished % BO == 0) {
				GCooks[j].SetBstarts(CurrentTimeStep);
				GCooks[j].SetInBreak(1);
			}
		}
	}
}

//This function determines which cooks should return from break based on their break time.
void Restaurant::CooksFromBreak(int CurrentTimeStep) {
	for (int j = 0; j < NC_Count; j++) {
		if (!(NCooks[j].isAvailable())) {
			if (NCooks[j].GetBstarts() + BN == CurrentTimeStep) {
				NCooks[j].SetInBreak(0);
				NCooks[j].setAvailabletime(CurrentTimeStep);
				NCooks[j].n_finished = 0;
			}
		}
	}
	for (int j = 0; j < VC_Count; j++) {
		if (!(VCooks[j].isAvailable())) {
			if (VCooks[j].GetBstarts() + BV == CurrentTimeStep) {
				VCooks[j].SetInBreak(0);
				VCooks[j].n_finished = 0;
				VCooks[j].setAvailabletime(CurrentTimeStep);
			}
		}
	}
	for (int j = 0; j < GC_Count; j++) {
		if (!(GCooks[j].isAvailable())) {
			if (GCooks[j].GetBstarts() + BG == CurrentTimeStep) {
				GCooks[j].SetInBreak(0);
				GCooks[j].n_finished = 0;
				GCooks[j].setAvailabletime(CurrentTimeStep);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


//The following functions are used to define the steps required in each of the three simulation modes.

//Interactive Simulation which needs a mouse click to go to the next timestep and then generate the OUTPUT TEXT file at the end.
void Restaurant::Interactive_simulation() {

	int ID = 1;
	
	pGUI->PrintMessage("Enter file name :");
	this->LoadFile(pGUI->GetString().c_str());

	//Loading cooks information
	NCooks = new Cook[NC_Count];
	for (int i = 0; i < NC_Count; i++) {
		NCooks[i].setID(ID++);
		NCooks[i].setType(TYPE_NRM);
		NCooks[i].setSpd(SN);
		NCooks[i].setBreakDue(BN);
		NCooks[i].setNorders(BO);
	}

	GCooks = new Cook[GC_Count];
	for (int i = 0; i < GC_Count; i++) {
		GCooks[i].setID(ID++);
		GCooks[i].setType(TYPE_VGAN);
		GCooks[i].setSpd(SG);
		GCooks[i].setBreakDue(BG);
		GCooks[i].setNorders(BO);
	}

	VCooks = new Cook[VC_Count];
	for (int i = 0; i < VC_Count; i++) {
		VCooks[i].setID(ID++);
		VCooks[i].setType(TYPE_VIP);
		VCooks[i].setSpd(SV);
		VCooks[i].setBreakDue(BV);
		VCooks[i].setNorders(BO);
	}


	pGUI->PrintMessage("Loading Events from the file...CLICK to continue");
	pGUI->waitForClick();
	int CurrentTimeStep = 0, aProms = 0; // The timestep at which cooks are initialized




	//as long as events queue is not empty yet  
	while (!(EventsQueue.isEmpty() && InServ_Norm_Orders.isEmpty() && InServ_Veg_Orders.isEmpty() && InServ_VIP_Orders.isEmpty()))
	{
		msgs = new string[20];
		amsg = 0;
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		aProms += autoprom(CurrentTimeStep);

		CooksFromBreak(CurrentTimeStep);
		VIP2Finished(CurrentTimeStep);
		Vegan2Finished(CurrentTimeStep);
		Normal2Finished(CurrentTimeStep);
		CooksToBreak(CurrentTimeStep);

		VIP2Service(CurrentTimeStep);
		Vegan2Service(CurrentTimeStep);
		Normal2Service(CurrentTimeStep);


		this->FillDrawingList();
		pGUI->UpdateInterface();
		this->UpdateStatusbar(CurrentTimeStep);
		delete[] msgs;

		//Sleep(4000);
	    pGUI->waitForClick();

		pGUI->ResetDrawingList();
		//advance timestep
		CurrentTimeStep++;

	}
	OutputFile(aProms);
	delete[]NCooks;
	delete[]VCooks;
	delete[]GCooks;
	

	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}



//Step By Step Simulation which goes to the next timestep after one second and then generate the OUTPUT TEXT file at the end.
void Restaurant::StepByStep_simulation() {

	int ID = 1;
	pGUI->PrintMessage("Enter file name :");
	this->LoadFile(pGUI->GetString().c_str());

	//Loading cooks information
	NCooks = new Cook[NC_Count];
	for (int i = 0; i < NC_Count; i++) {
		NCooks[i].setID(ID++);
		NCooks[i].setType(TYPE_NRM);
		NCooks[i].setSpd(SN);
		NCooks[i].setBreakDue(BN);
		NCooks[i].setNorders(BO);
	}

	
	GCooks = new Cook[GC_Count];
	for (int i = 0; i < GC_Count; i++) {
		GCooks[i].setID(ID++);
		GCooks[i].setType(TYPE_VGAN);
		GCooks[i].setSpd(SG);
		GCooks[i].setBreakDue(BG);
		GCooks[i].setNorders(BO);
	}

	VCooks = new Cook[VC_Count];
	for (int i = 0; i < VC_Count; i++) {
		VCooks[i].setID(ID++);
		VCooks[i].setType(TYPE_VIP);
		VCooks[i].setSpd(SV);
		VCooks[i].setBreakDue(BV);
		VCooks[i].setNorders(BO);
	}

	pGUI->PrintMessage("Loading Events from the file...CLICK to continue");
	pGUI->waitForClick();
	int CurrentTimeStep = 0, aProms = 0; // The timestep at which cooks are initialized




	//as long as events queue is not empty yet  
	while (!(EventsQueue.isEmpty() && InServ_Norm_Orders.isEmpty() && InServ_Veg_Orders.isEmpty() && InServ_VIP_Orders.isEmpty()))
	{
		msgs = new string[20];
		amsg = 0;
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		aProms += autoprom(CurrentTimeStep);

		CooksFromBreak(CurrentTimeStep);
		VIP2Finished(CurrentTimeStep);
		Vegan2Finished(CurrentTimeStep);
		Normal2Finished(CurrentTimeStep);
		CooksToBreak(CurrentTimeStep);

		VIP2Service(CurrentTimeStep);
		Vegan2Service(CurrentTimeStep);
		Normal2Service(CurrentTimeStep);



		this->FillDrawingList();
		pGUI->UpdateInterface();
		this->UpdateStatusbar(CurrentTimeStep);
		Sleep(1000);
		delete[] msgs;

		pGUI->ResetDrawingList();
		//advance timestep
		CurrentTimeStep++;

	}
	OutputFile(aProms);
	delete[]NCooks;
	delete[]VCooks;
	delete[]GCooks;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

//Silent Simulation which will only generate the OUTPUT TEXT file.
void Restaurant::Silent_simulation()
{
	int ID = 0;
	pGUI->PrintMessage("Enter file name :");
	this->LoadFile(pGUI->GetString().c_str());

	//Loading cooks information
	NCooks = new Cook[NC_Count];
	for (int i = 0; i < NC_Count; i++) {
		NCooks[i].setID(ID++);
		NCooks[i].setType(TYPE_NRM);
		NCooks[i].setSpd(SN);
		NCooks[i].setBreakDue(BN);
		NCooks[i].setNorders(BO);
	}

	GCooks = new Cook[GC_Count];
	for (int i = 0; i < GC_Count; i++) {
		GCooks[i].setID(ID++);
		GCooks[i].setType(TYPE_VGAN);
		GCooks[i].setSpd(SG);
		GCooks[i].setBreakDue(BG);
		GCooks[i].setNorders(BO);
	}

	VCooks = new Cook[VC_Count];
	for (int i = 0; i < VC_Count; i++) {
		VCooks[i].setID(ID++);
		VCooks[i].setType(TYPE_VIP);
		VCooks[i].setSpd(SV);
		VCooks[i].setBreakDue(BV);
		VCooks[i].setNorders(BO);
	}

	pGUI->PrintMessage("Loading Events from the file...CLICK to generate");
	pGUI->waitForClick();
	int CurrentTimeStep = 0, aProms = 0; // The timestep at which cooks are initialized

	while (!(EventsQueue.isEmpty() && InServ_Norm_Orders.isEmpty() && InServ_Veg_Orders.isEmpty() && InServ_VIP_Orders.isEmpty()))
	{
		msgs = new string[20];
		amsg = 0;

		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		aProms += autoprom(CurrentTimeStep);

		CooksFromBreak(CurrentTimeStep);
		VIP2Finished(CurrentTimeStep);
		Vegan2Finished(CurrentTimeStep);
		Normal2Finished(CurrentTimeStep);
		CooksToBreak(CurrentTimeStep);

		VIP2Service(CurrentTimeStep);
		Vegan2Service(CurrentTimeStep);
		Normal2Service(CurrentTimeStep);

		//advance timestep
		CurrentTimeStep++;
	}


	OutputFile(aProms);
	delete[]NCooks;
	delete[]VCooks;
	delete[]GCooks;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}
