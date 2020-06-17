#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	int count;	//Number of nodes in the list

public:

	LinkedList();
	~LinkedList();

	void DeleteAll();
	bool isEmpty() const;
	T* toArray(int& count);
	void InsertBeg(const T& data);
	void InsertEnd(const T& data);
	bool DeleteNodes(const T& value);
	bool DeleteNode(const T& value);
	int CountOccurance(const T& value);
	bool Find(T Key);
	int getCount();

};



template<typename T>
inline LinkedList<T>::LinkedList()
{
	count = 0;
	Head = nullptr;
}


//List is being desturcted ==> delete all items in the list
template<typename T>
inline LinkedList<T>::~LinkedList()
{
	DeleteAll();
}

////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this linkedlist is empty.

Input: None.
Output: True if the linkedlist is empty; otherwise false.
*/

template<typename T>
 bool LinkedList<T>::isEmpty() const
{
	 if (Head == nullptr)
		 return true;
	 else
		 return false;
 }

 ////////////////////////////////////////////////////////////////////////

 template<typename T>
 inline T* LinkedList<T>::toArray(int& count)
 {
	 count = 0;

	 if (!Head)
		 return nullptr;
	 //counting the no. of items in the Linkedlist
	 Node<T>* p = Head;
	 while (p)
	 {
		 count++;
		 p = p->getNext();
	 }


	 T* Arr = new T[count];
	 p = Head;
	 for (int i = 0; i < count; i++)
	 {
		 Arr[i] = p->getItem();
		 p = p->getNext();
	 }
	 return Arr;
 }

 ///////////////////////////////////////////////////////////////////////////////////////////
		/*
		* Function: InsertBeg.
		* Creates a new node and adds it to the beginning of a linked list.
		*
		* Parameters:
		*	- data : The value to be stored in the new node.
		*/
 template<typename T>
 inline void LinkedList<T>::InsertBeg(const T& data)
 {
	 Node<T>* R = new Node<T>(data);
	 R->setNext(Head);
	 Head = R;
	 count++;
 }


 ///////////////////////////////////////////////////////////////////////////


	 //[1]InsertEnd 
	 //inserts a new node at end if the list
 template<typename T>
 inline void LinkedList<T>::InsertEnd(const T& data)
 {
	 Node<T>* p = Head;
	 Node<T>* R = new Node<T>(data);

	 if (isEmpty()) {
		 Head = R;
		 count++;
	 }
	 else {
		 while (p->getNext()) {
			 p = p->getNext();
		 }
		 p->setNext(R);
		 count++;
	 }
 }
 ///////////////////////////////////////////////////////////////////////////////////
 //[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
 template<typename T>
 inline bool LinkedList<T>::DeleteNodes(const T& value)
 {
	 Node<T>* p = Head;
	 Node<T>* A = Head->getNext();
	 Node<T>* C = Head;

	 int i = 0;
	 if (!Head || !(Head->getNext()))
		 return 0;

	 if (p->getItem() == value) {
		 Head = p->getNext();
		 C = p;
		 p = p->getNext();
		 A = A->getNext();
		 delete C;
		 count--;
		 i++;
	 }
	 while (A->getNext()) {
		 if (A->getItem() == value) {
			 C = A;
			 p->setNext(A->getNext());
			 A = p->getNext();
			 delete C;
			 count--;
			 i++;
		 }
		 else {
			 p = p->getNext();
			 A = A->getNext();
		 }
	 }

	 if (!A->getNext() && A->getItem() == value) {
		 p->setNext(nullptr);
		 A = p->getNext();
		 delete A;
		 count--;
		 i++;
	 }
	 if (p->getItem() == value) {
		 delete p;
		 Head = nullptr;
		 count--;
		 i++;
	 }

	 if (i == 0)
		 return 0;
	 else
		 return 1;
 }

 //[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
 template<typename T>
 inline bool LinkedList<T>::DeleteNode(const T& value)
 {
	 Node<T>* p = Head;
	 Node<T>* A = Head->getNext();

	 if (p->getItem() == value) {
		 Head = p->getNext();
		 delete p;
		 count--;
		 return 1;
	 }
	 else {
		 while (A->getItem() != value) {

			 if (A->getNext()) {
				 p = p->getNext();
				 A = A->getNext();
			 }
			 else return 0;
		 }
	 }
	 p->setNext(A->getNext());
	 delete A;
	 count--;
	 return 1;
 }

 //[3]CountOccurance
	//returns how many times a certain value appeared in the list
 template<typename T>
 inline int LinkedList<T>::CountOccurance(const T& value)
 {
	 Node<T>* p = Head;
	 int c = 0;
	 if (p->getItem() == value)
		 c++;
	 while (p->getNext()) {
		 p = p->getNext();
		 if (p->getItem() == value)
			 c++;
	 }

	 return c;
 }


 /////////////////////////////////////////////////////////////////////////////////////////////
 //[2]Find 
 //searches for a given value in the list, returns true if found; false otherwise.


 template<typename T>
 inline bool LinkedList<T>::Find(T Key)
 {
	 Node<T>* p = Head;

	 while (p->getNext()) {
		 p = p->getNext();
		 if (p->getItem() == Key)
			 return 1;
	 }
	 return 0;
 }

 template<typename T>
 inline int LinkedList<T>::getCount()
 {
	 return count;
 }

 ////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
 template<typename T>
 inline void LinkedList<T>::DeleteAll()
 {
	 Node<T>* P = Head;
	 while (Head)
	 {
		 P = Head->getNext();
		 delete Head;
		 Head = P;
	 }
	 count = 0;
 }

 



#endif	
