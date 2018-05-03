#pragma once

#include <iostream>

struct Buffer;	// forward declaration

_Monitor Printer {
	Buffer* buffer;				// array of buffers
	unsigned int hbiu;			// highest buffer in use - used by flush()
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	void flush();              // prints out every buffer and marks them as empty (.full = false)
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    // getBuff
    // arguments: object kind and (optionally) it's id number (if student, vm, or courier)
    // returns: object's index in the buffer array
	unsigned int getBuff(Kind kind, int id = 0);
};