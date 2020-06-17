#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Events\Event.h"


#include "Order.h"

// it is the maestro of the project
class Restaurant
{
private:
	GUI* pGUI;

	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	
	//Lists and Queues of waiting orders
	LinkedList<Order*> Wait_Norm_Orders; 
	Queue<Order*> Wait_Veg_Orders;
	PriorityQueue<Order*> Wait_VIP_Orders;

	//Lists and Queues of waiting orders
	LinkedList<Order*> InServ_Norm_Orders;
	Queue<Order*> InServ_Veg_Orders;
	LinkedList<Order*> InServ_VIP_Orders;

	Queue<Order*> Finished_Orders;	//Queue of all finished orders

	//Arrays of Cooks
	Cook* NCooks;
	Cook* GCooks;
	Cook* VCooks;
	int VC_Count, GC_Count, NC_Count; //Number of cooks
	int SN, SG, SV, BO, BN, BG, BV; // speed of cooks and break info.
	string* msgs; //msgs to display the type and ID of the cooks with the type and ID of assigned order
	int amsg; // number  msgs that displayes each timestep
	int servN = 0; //Number of served normal orders 
	int servG = 0; //Number of served Vegan orders 
	int servV = 0;	//Number of served VIP orders 
	int auto_p; //Auto-Promotion after waiting timesteps
public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation(); // Simulates the Restaurant
	Cook* AvailableCooks(); // returns an array of the available cooks
	void CancelOrder(int id); // Cancels order from normal waiting list
	int* getAvailableCook() const; // returns an array of number of available cooks in the format of [N, G, V]
	int getNormWait(); // returns the number of normal waiting orders
	int getVIPWait(); // returns the number of VIP waiting orders
	int getVegWait(); // returns the number of vegan waiting orders
	void FillDrawingList(); // Fills the data that will be drown
	void LoadFile(string f); // Reads the input file and fill the events queue
	void OutputFile(int aProms);
	void UpdateStatusbar(int t); // Updates the status bar in each timestep
	void SetCookFree(int Order_ID, ORD_TYPE type); // Makes the cook available after he finishes the order
	void AddtoOrderQueue(Order* oQ); // adds orders to its suitable place while executing events
	void promevent(int id, double money);
	int autoprom(int t);

	void Interactive_simulation();
	void StepByStep_simulation();
	void Silent_simulation();

	void Normal2Service(int CurrentTimeStep);
	void Normal2Finished(int CurrentTimeStep);
	void VIP2Service(int CurrentTimeStep);
	void VIP2Finished(int CurrentTimeStep);
	
	void Vegan2Service(int CurrentTimeStep);
	void Vegan2Finished(int CurrentTimeStep);

	void CooksToBreak(int CurrentTimeStep);
	void CooksFromBreak(int CurrentTimeStep);






};

#endif