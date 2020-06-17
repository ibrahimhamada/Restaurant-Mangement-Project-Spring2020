//#include "Drawing.h"
#include "Rest\Restaurant.h"
#include "GUI\GUI.h"

void main()
{

	Restaurant* pRest = new Restaurant; //Initiates a new restaurant
	pRest->RunSimulation(); //Simulates the restaurant

	delete pRest; //Closes the restaurant by deleting the pointer

	return;
}
 