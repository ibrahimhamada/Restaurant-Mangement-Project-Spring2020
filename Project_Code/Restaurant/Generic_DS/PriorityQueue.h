#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_


#include "PriorityNode.h"

template <typename T>
class PriorityQueue
{
private:
	PriorityNode<T>* Head;
	int count = 0;	
public:
	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry,int p);
	bool dequeue(T& frntEntry);
	int getCount() const;
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array if items)
	~PriorityQueue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: PriorityQueue()
The constructor of the Queue class.

*/

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	Head = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool PriorityQueue<T>::isEmpty() const
{
	if (Head == nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry, int p)
{
	PriorityNode<T>* newNodePtr = new PriorityNode<T>(newEntry,p);
	// Insert the new node
	if (isEmpty())
	{
		Head = newNodePtr;
	} // The queue is empty
	else
	{
		if (p > Head->getPriority())
		{
			newNodePtr->setNext(Head);
			Head = newNodePtr;
		}
		else
		{
			PriorityNode<T>* R = Head;
			while (!R->getNext() && R->getNext()->getPriority() > p)
				R = R->getNext();
			newNodePtr->setNext(R->getNext());
			R->setNext(newNodePtr);
		}
	}
	count++;
	return true;
} // end enqueue


  /////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*Function: dequeue
  Removes the front of this queue. That is, removes the item that was added
  earliest.

  Input: None.
  Output: True if the operation is successful; otherwise false.
  */

template <typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PriorityNode<T>* nodeToDeletePtr = Head;
	frntEntry = Head->getItem();
	Head = Head->getNext();
	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;
	count--;

	return true;

}

template<typename T>
int PriorityQueue<T>::getCount() const
{
	return count;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = Head->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
}


/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (nullptr if Queue is empty)
*/

template <typename T>
T* PriorityQueue<T>::toArray(int& c)
{
	c = count;

	if (!Head)
		return nullptr;
	//counting the no. of items in the Queue
	PriorityNode<T>* p = Head;


	T* Arr = new T[c];
	p = Head;
	for (int i = 0; i<count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}


#endif
