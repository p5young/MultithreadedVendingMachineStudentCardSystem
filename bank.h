#pragma once

_Monitor Bank {
	unsigned int *balances;	// list of account balances
	uCondition *benches;	// list of benches for couriers to wait on if they attempt withdrawl & insufficient funds
  public:
    Bank( unsigned int numStudents );						// constructor
    ~Bank();												// destructor
    void deposit( unsigned int id, unsigned int amount );	// deposit funds
    void withdraw( unsigned int id, unsigned int amount );	// withdraw funds
};