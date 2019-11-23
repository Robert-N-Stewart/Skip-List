// robert stewart
// skip list
// the skip list acts as a more efficent list 
// it has search and get complexity at big O of logN whereas linked list has complexity of big O of n
// has several functions for mutating the list
// insert, clear, remove
// has sever function for acessing the list
// get, isEmpty, show, search, size 
// templatized to accept multiple types of data
// uses nodes for internal storage

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int LEVEL = 8;		// max level of the Skip list


//  node object of the skip list
// templated to store any type of item
// has pointers to up, next, prev, and down nodes
template<typename Object>
class SLNode {
public:

	// data members to store linked lists data, and pointers to up, down, next and previous Nodes
	Object data;
	SLNode<Object>* next;
	SLNode<Object>* prev;
	SLNode<Object>* up;
	SLNode<Object>* down;

	// default constructor for nodes
	// initializes all data to NULL 
	// used in the provided init function
	SLNode<Object>() {
		this->data = NULL;
		this->next = NULL;
		this->prev = NULL;
		this->up = NULL;
		this->down = NULL;
	}

	// constructor that accepts two data members, one for its data type 
	// and one for the pointer to the next node
	SLNode<Object>(Object data, SLNode<Object>* down) {
		this->data = data;
		this->next = NULL;
		this->prev = NULL;
		this->up = NULL;
		this->down = down;
	}

	// constructor that accepts parameters for each one of its data mebers
	SLNode<Object>(Object data, SLNode<Object>* next, SLNode<Object>* prev, SLNode<Object>* up, SLNode<Object>* down) {
		this->data = data;
		this->next = next;
		this->prev = prev;
		this->up = up;
		this->down = down;
	}
};

// skip list
// the skip list acts as a more efficent list 
// it has search and get complexity at big O of logN whereas linked list has complexity of big O of n
// has several functions for mutating the list
// insert, clear, remove
// has sever function for acessing the list
// get, isEmpty, show, search, size 
// templatized to accept multiple types of data
// uses nodes for internal storage 
template<typename Object>
class SkipList {

public:
	SkipList();		// default constructor
	SkipList(const SkipList&);
	SkipList<Object>& operator=(const SkipList<Object>&);		// compy constructor
	~SkipList();		// destructs the list
	SLNode<Object>* get(Object&);		// returns a pointer to the node in the list
	bool search(Object&);		// searches for the passed object in the list
	bool insert(Object&);		// inserts passed object into the list
	bool remove(Object&);		// removes passed object form list
	int size() const;		// returns the size of the list
	void show() const;		// displays the list
	bool isEmpty() const;		// returns true if list is empty
	void clear();		// clears list
	bool operator==(const SkipList<Object>&) const;		// used to comare two lists

private:

	void init();
	SLNode<Object>* header[LEVEL];		// array of pointers to Nodes used as dummy nodes for the begining of list

};

// --------------------------------------------------------------------
// default constructor
// initializes the lists dummy nodes by calling init
template<typename Object>
SkipList<Object>::SkipList()
{
	init();
}

// --------------------------------------------------------------------
// constructor
// initializes the lists dummy nodes by calling init
template<typename Object>
SkipList<Object>::SkipList(const SkipList & passedList)
{
	init();
	this->operator=(passedList);
}

// --------------------------------------------------------------------
// copy constructor for compying one list to another
// copies all levels completely
template<typename Object>
SkipList<Object>& SkipList<Object>::operator=(const SkipList<Object> & list)
{
	this->clear();		// clears list for new list

	SLNode<Object>* current = header[0];
	SLNode<Object>* current2 = list.header[0]->next;
	while (current2->data != NULL) {		// for every node of passed list
		current->next = new SLNode<Object>(current2->data, current->next, current, NULL, NULL);
		current->next->next->prev = current->next;		// make a node from it and re assign pointers
		current = current->next;

		SLNode<Object>* current2Up = current2->up;
		SLNode<Object>* oldcurrent = current;		// for moving onto the next column

		while (current2Up != NULL) {		// then make new new nodes from all of its upper levels
			current->up = new SLNode<Object>(current2->data, current);
			SLNode<Object>* prevWithUp = current->prev;
			SLNode<Object>* nextWithUp = current->next;		// re assigning pointers to it
			current = current->up;

			while (prevWithUp->data != NULL && prevWithUp->up == NULL) {
				prevWithUp = prevWithUp->prev;
			}

			prevWithUp = prevWithUp->up;
			current->prev = prevWithUp;
			prevWithUp->next = current;


			while (nextWithUp->data != NULL && nextWithUp->up == NULL) {
				nextWithUp = nextWithUp->next;
			}

			nextWithUp = nextWithUp->up;
			current->next = nextWithUp;
			nextWithUp->prev = current;

			current2Up = current2Up->up;
		}

		current = oldcurrent;		 // move to the next column
		current2 = current2->next;		// move to the next colum of the next list
	}
	return *this;

}

// --------------------------------------------------------------------
// destructs the list complety
// dealocates all memory to the list
// does not dealocate memory to the objects contained within each of the nodes
template<typename Object>
SkipList<Object>::~SkipList()
{
	SLNode<Object>* current = header[0];
	SLNode<Object>* nextNode = current->next;
	while (current != NULL) {		// go through each of the rows 
		while (current != NULL) {		// deleting each of the colums
			SLNode<Object>* nodeToDelete = current;
			current = current->up;
			delete nodeToDelete;
		}
		current = nextNode;
		if (current != NULL) {
			nextNode = nextNode->next;
		}
	}

}

// --------------------------------------------------------------------
// get
// retieves the node containing the passed object from the list or the node of the node that comes before it 
// node returned will always be on the base level
template<typename Object>
SLNode<Object>* SkipList<Object>::get(Object& object)
{
	SLNode<Object>* current = header[LEVEL - 1];
	while (current != NULL) {		// go through each row

		while (current->next->data != NULL && current->next->data <= object) {		// move to the right of the list
			current = current->next;		// until everything to the right is greater
			if (current->data == object) {		// unless you find the item
				return current;
			}
		}
		if (current->down == NULL) {		// if at the base row break and dont go down
			break;
		}
		current = current->down;
	}

	while (current->next->data != NULL && current->next->data <= object) {		// search through the base row
		current = current->next;
		if (current->data == object) {
			return current;
		}
	}

	return current;
}

// --------------------------------------------------------------------
// search
// simply searches for the item by returning get->data if the object is found
// or returns false otherwise
template<typename Object>
inline bool SkipList<Object>::search(Object & object)
{
	SLNode<Object>* current = get(object);		// finds node to object or not
	if (current->data == object) {		// if it was the right node
		return true;
	}
	else {
		return false;
	}
}

// --------------------------------------------------------------------
// inserts the object into the sorted list
// returns true if the insert is sucessful
// will only be unsucessful if the object is already currently in the list
template<typename Object>
bool SkipList<Object>::insert(Object& object)
{
	SLNode<Object>* current = get(object);
	if (current->data == object) {		// if the list already contains the item
		return false;
	}
	else {		// else create a new node as the gotten nodes next pointer
		current->next = new SLNode<Object>(object, current->next, current, NULL, NULL);
		current = current->next;
		current->next->prev = current;		// reallocate pointers
	}

	int level = 0;
	srand(time(0));

	// creates nodes containing the object
	// creation depends on a 50% probability 
	while (rand() % 2 == 0 && level < LEVEL) {
		current->up = new SLNode<Object>(object, current);		// create upper node
		SLNode<Object>* prevWithUp = current->prev;		// make pointers to find the uppers prev and next
		SLNode<Object>* nextWithUp = current->next;
		current = current->up;


		while (prevWithUp->data != NULL && prevWithUp->up == NULL) {		// find node to uppers prev
			prevWithUp = prevWithUp->prev;
		}

		prevWithUp = prevWithUp->up;
		current->prev = prevWithUp;
		prevWithUp->next = current;


		while (nextWithUp->data != NULL && nextWithUp->up == NULL) {		// find node to uppers next 
			nextWithUp = nextWithUp->next;
		}

		nextWithUp = nextWithUp->up;		// realocate pointers
		current->next = nextWithUp;
		nextWithUp->prev = current;
		level++;

	}		// reflip for another chance at making another up node
	return true;

}

// --------------------------------------------------------------------
// remove
// removes the passed object from the list
template<typename Object>
bool SkipList<Object>::remove(Object & object)
{
	SLNode<Object>* current = get(object);		// gets the 
	if (current->data != object) {		// if the list does not contain it simply return false
		return false;
	}
	else {
		while (current != NULL) {		// else delete the found node at the top of its col
			current->prev->next = current->next;
			current->next->prev = current->prev;
			SLNode<Object>* temp = current;
			current = current->down;		//works its way down the col
			delete temp;
		}
		return true;
	}
}

// --------------------------------------------------------------------
// size
// returns the size of the list
template<typename Object>
int SkipList<Object>::size() const		// returns the base size of the list
{
	SLNode<Object>* current = h[0]->next;
	int s = 0;
	while (current->data != NULL) {
		s++;
		current = current->next;
	}
	return s;
}

// --------------------------------------------------------------------
// show
// displays the list
// provided by Rob Nash
// dummy cols are displayed as -inf and +inf
template<typename Object>
void SkipList<Object>::show() const
{
	cout << "contents:" << endl;
	for (SLNode<Object>* col = header[0]; col != NULL; col = col->next) {
		SLNode<Object>* row = col;
		for (int level = 0; row != NULL && level < LEVEL; level++) {
			if (row->prev == NULL)
				cout << "-inf\t";
			else if (row->next == NULL)
				cout << "+inf\t";
			else   cout << row->data << "\t";
			row = row->up;
		}     cout << endl;
	}
}

// --------------------------------------------------------------------
// isEmpty
// returns true if the list is empty
template<typename Object>
bool SkipList<Object>::isEmpty() const
{
	return (header[0]->next->data == NULL);		// checks if dummy nodes at base are pointing to each other
}

// --------------------------------------------------------------------
// clear
// clears the list deleing all nodes in it
// except the dummy nodes for further use of the list
template<typename Object>
void SkipList<Object>::clear()		// clears all nodes from the list exept the dummy nodes
{
	SLNode<Object>* current;
	current = header[0];
	current = current->next;
	SLNode<Object>* nextNode = current->next;
	while (current->data != NULL) {		// go through each of the cols 
		while (current != NULL) {		// go through each element of the colum 
			SLNode<Object>* nodeToDelete = current;
			current = current->up;
			delete nodeToDelete;		// deleting the list
		}	
		current = nextNode;
		nextNode = nextNode->next;
	}

	int k = 0;
	while (current != NULL && k <= LEVEL) {		// reassign the dummy nodes to point to each other
		header[k]->next = current;
		current->prev = header[k];
		current = current->up;
		k++;
	}

}

// --------------------------------------------------------------------
// operator==
// compares two lists to each other 
// returns true if both lists contain the same base of elements in the bottom row
template<typename Object>
bool SkipList<Object>::operator==(const SkipList<Object>& list1) const
{
	if (list1.size() != this->size) {
		return false;
	}
	else {		// go through lists parralelly 
		SLNode<Object>* current = header[0]->next;
		SLNode<Object>* current2 = list1.header[0]->next;
		while (current->data != NULL) {
			if (current->data != current2->data) {		// if parralel elements are not the same return false
				return false;
			}
			else {
				current = current->next;
				current2 = current2->next;
			}
		}
		return true;
	}

}

// --------------------------------------------------------------------
// init
// initialiZes the lists dummy nodes
// provided by Rob Nash
template<typename Object>
void SkipList<Object>::init()
{
	for (int i = 0; i < LEVEL; i++) {  // for each level     
									   // create the left most dummy nodes     
		header[i] = new SLNode<Object>;
		header[i]->prev = NULL;
		header[i]->down = (i > 0) ? header[i - 1] : NULL;
		header[i]->up = NULL;
		if (i > 0) header[i - 1]->up = header[i];

		// create the right most dummy nodes
		header[i]->next = new SLNode<Object>;
		header[i]->next->next = NULL;
		header[i]->next->prev = header[i];
		header[i]->next->down = (i > 0) ? header[i - 1]->next : NULL;
		header[i]->next->up = NULL;
		if (i > 0) header[i - 1]->next->up = header[i]->next;
	}
}



#endif 
