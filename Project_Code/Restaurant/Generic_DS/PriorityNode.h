
#ifndef __PRIORITY_NODE_H_
#define __PRIORITY_NODE_H_

template < typename T>
class PriorityNode
{
private:
	T item; // A data item
	int priority;
	PriorityNode<T>* next; // Pointer to next node
public:
	PriorityNode();
	PriorityNode(const T & r_Item, int p);	//passing by const ref.
	PriorityNode(const T & r_Item, int p, PriorityNode<T>* nextNodePtr);
	void setItem(const T & r_Item);
	void setNext(PriorityNode<T>* nextNodePtr);
	void setPriority(int p);
	int getPriority() const;
	T getItem() const;
	PriorityNode<T>* getNext() const;
}; // end Node


template < typename T>
PriorityNode<T>::PriorityNode()
{
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item, int p)
{
	item = r_Item;
	priority = p;
	next = nullptr;
}

template < typename T>
PriorityNode<T>::PriorityNode(const T& r_Item,int p, PriorityNode<T>* nextNodePtr)
{
	item = r_Item;
	priority = p;
	next = nextNodePtr;
}
template < typename T>
void PriorityNode<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void PriorityNode<T>::setNext(PriorityNode<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template<typename T>
void PriorityNode<T>::setPriority(int p)
{
	priority = p;
}

template<typename T>
int PriorityNode<T>::getPriority() const
{
	return priority;
}

template < typename T>
T PriorityNode<T>::getItem() const
{
	return item;
}

template < typename T>
PriorityNode<T>* PriorityNode<T>::getNext() const
{
	return next;
}

#endif