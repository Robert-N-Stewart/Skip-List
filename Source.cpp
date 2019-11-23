#include "skipList.h"
// driver that completly tests each one of Skip Lists functions
// displays skip lists current status with provided show functoin that outputs skip lists current status
// tests skip lists insert, retrieve, remove, and other functions 
using namespace std;

int main() {

	SkipList<int> skiplist;

	srand(time(0));

	int k;

	// tests insertation of multile objects
	for (int i = 0; i < 6; i++) {
		k = rand() % 100000;
		skiplist.insert(k);
	}

	// displays list to ensure that all inserts are ordered and look correct 
	skiplist.show();

	k = 1;

	// tests to see if skip list search function returns the correct value when searched objects is not in the list
	cout << "tests to see if 1 is in the skip list. There is a 99.99% chance that 1 is not in the skip list" << endl;
	if (!skiplist.search(k)) {
		cout << "1 is not in the skip list" << endl;
	}
	else {
		cout << "1 is in the skip list" << endl;
	}

	skiplist.insert(k);

	// tests to see if an skip list search funtion returns the correct value when searched object is in the list
	if (skiplist.search(k)) {
		cout << "correct, 1 is now in the skip list" << endl;
	}
	else {
		cout << "incorrect 1 is should now be in the skip list" << endl;
	}

	skiplist.show();

	// tests to see that insert returns false for a object already inserted
	if (!skiplist.insert(k)) {
		cout << "correct, 1 is already in the skip list" << endl;
	}
	else {
		cout << "incorrect 1 is already  be in the skip list" << endl;
	}



	// tests to ensure that remove prefores correctly when object to remove is in the List
	if (skiplist.remove(k)) {
		cout << "1 should be removed check display to be sure" << endl;
	}
	else {
		cout << " 1 was not removed correctly" << endl;
	}

	skiplist.show();		// shows list to insure that remove was correct

							// tests to ensure that remove prefores correctly when object is not in the List
	if (!skiplist.remove(k)) {
		cout << "Correct, 1 was already removed, if above statment is true" << endl;
	}
	else {
		cout << " False, 1 should already have been removed" << endl;
	}

	// tests copy constructor and operater =
	// showing of list2 after should be the same as the showing of list1 previously
	SkipList<int> skiplist2(skiplist);

	cout << "check display to insure that copy was correct" << endl;
	skiplist2.show();

	// tests to make sure skip lists clear words correctly 
	skiplist.clear();

	// tests to make sure that skip list is now empty
	if (skiplist.isEmpty()) {
		cout << "correct skip list is how empty" << endl;
	}
	else {
		cout << " incorrect, skip list should now be empty" << endl;
	}


	return 0;
}