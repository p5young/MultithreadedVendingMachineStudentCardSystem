#include "bank.h"

// constructor
Bank::Bank( unsigned int numStudents ) {
	balances = new unsigned int[numStudents];			// declare accounts
	for (unsigned int i = 0 ; i < numStudents ; ++i)	// initialize to 0
		balances[i] = 0;

	benches = new uCondition[numStudents];				// declare benches
}; // constructor

// destructor
Bank::~Bank() {
	delete[] balances;	// delete accounts
	delete[] benches;	// delete benches
} // destructor

// deposits money into id's account, and wakes up one person waiting to withdraw from that acct.
void Bank::deposit( unsigned int id, unsigned int amount ) {
	balances[id] += amount;			// update balance
	benches[id].signal();			// wake up one waiting person
} // deposit

// withdraws money from id's account, waits if insufficient funds, wakes up next waiter if successful
void Bank::withdraw( unsigned int id, unsigned int amount ) {
	while (balances[id] < amount)	// block if insufficient funds
		benches[id].wait();
	balances[id] -= amount;			// withdraw money
} // withdraw
