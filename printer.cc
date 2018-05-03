#include "printer.h"

using namespace std;

// Output buffer
struct Buffer {
    bool full;			// true if the buffer has something to write
	Printer::Kind kind;	// the kind of the item being printed
    unsigned int lid;	// student, vending machine, or courier's list id
    char state;			// letter to be printed
    int value1;			// first number to be printed (if applicable)
    int value2;			// second number to be printed (if applicable)
};

// << operator for Buffer struct
ostream& operator<<(ostream& os, const Buffer& buf) {
	int numval = 0;			// Number of values to print after the state, separated by comma
	switch(buf.state){
		case 'F':
		case 'W':
		case 'r':
			break;			// numval = 0
		case 'V':
			numval = 1;
			break;			// numval = 1
		case 'C':
		case 'T':
		case 'N':
		case 'd':
		case 'U':
		case 'a':
		case 'B':
		case 'A':
		case 't':
			numval = 2;
			break;			// numval = 2
		case 'S':
			if (buf.kind == Printer::Vending)
				numval = 1;
			else if (buf.kind == Printer::Student)
				numval = 2;
			break;
		case 'D':
			if (buf.kind == Printer::Groupoff)
				numval = 1;
			else
				numval = 2;
			break;
		case 'G':
			if (buf.kind == Printer::BottlingPlant)
				numval = 1;
			else
				numval = 2;
			break;
		case 'P':
			if (buf.kind == Printer::Truck)
				numval = 1;
			break;
		case 'L':
			if (buf.kind == Printer::Courier)
				numval = 1;
			break;
		case 'R':
			if (buf.kind == Printer::NameServer)
				numval = 1;
			break;
		default: 
			cerr << "Unknown State: " << buf.state << endl;	// this message should never print
	}
	os << buf.state;
	if (numval >= 1)
		os << buf.value1;
	if (numval == 2) {
		os << ',' << buf.value2;
	}

    return os;
} // buffer << operator

// print out the buffers and mark full as false
void Printer::flush() {
	for ( unsigned int i = 0 ; i <= hbiu ; ++i ) {
		if (buffer[i].full) {								// check if buffer is full
			cout << buffer[i];								// print out the buffer
			buffer[i].full = false;							// mark buffer as cleared
		}
		if (i < hbiu) cout << "\t";							// if this isn't the highest-buffer-in-use, place a tab
	}
	cout << endl;											// buffer flushed, print newline

	hbiu = 0;												// reset highest buffer in use
} // flush

// returns the index for the buffer array based on kind and id number
unsigned int Printer::getBuff(Kind kind, int id) {
	switch (kind) {
		case Parent:
			return 0;
		case Groupoff:
			return 1;
		case WATCardOffice:
			return 2;
		case NameServer:
			return 3;
		case Truck:
			return 4;
		case BottlingPlant:
			return 5;
		case Student:
			return 6 + id;
		case Vending:
			return 6 + numStudents + id;
		case Courier:
			return 6 + numStudents + numVendingMachines + id;
		default:
			cerr << "getBuff failed to identify Kind" << endl;
			return INT_MAX;
	}
} // getBuff

// constructor
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) : 
	numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers)
{
	unsigned int columns = 6 + numStudents + numVendingMachines + numCouriers;

	// Print out column headers
	cout << "Parent" << '\t'
		<< "Gropoff" << '\t'
		<< "WATOff" << '\t'
		<< "Names" << '\t'
		<< "Truck" << '\t'
		<< "Plant" << '\t';
	for (unsigned int i = 0 ; i < numStudents ; ++i) {
		cout << "Stud" << i << '\t';
	}
	for (unsigned int i = 0 ; i < numVendingMachines ; ++i) {
		cout << "Mach" << i << '\t';
	}
	for (unsigned int i = 0 ; i < numCouriers ; ++i) {
		cout << "Cour" << i;
		if (i < numCouriers - 1) cout << '\t';
	}
	cout << endl;
	for (unsigned int i = 0 ; i < columns ; ++i) {
		cout << "*******";
		if (i < columns - 1) cout << '\t';
	}
	cout << endl;

	// declare array of buffers
    buffer = new Buffer[columns];

    // initialize buffers
    for ( unsigned int i = 0 ; i < columns ; ++i ) {
    	buffer[i].full = false;
    }

    // assign buffer kinds
    buffer[0].kind = Parent;
    buffer[1].kind = Groupoff;
    buffer[2].kind = WATCardOffice;
    buffer[3].kind = NameServer;
    buffer[4].kind = Truck;
    buffer[5].kind = BottlingPlant;
    int pos = 6;	// position in the buffer
    for (unsigned int i = 0 ; i < numStudents ; ++i)
    	buffer[pos++].kind = Student;
    for (unsigned int i = 0 ; i < numVendingMachines ; ++i)
    	buffer[pos++].kind = Vending;
    for (unsigned int i = 0 ; i < numCouriers ; ++i)
    	buffer[pos++].kind = Courier;

    hbiu = 0;	// highest buffer in use = 0;
}; // constructor

// destructor
Printer::~Printer() {
	if (hbiu > 0 || buffer[0].full) flush();		// clear buffer, if necessary											
	delete[] buffer;								// free memory
	cout << "***********************" << endl;		// print footer
} // destructor

void Printer::print( Kind kind, char state ) {
	unsigned int id = getBuff(kind);	// get buffer id
	if (buffer[id].full) flush();		// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
}

void Printer::print( Kind kind, char state, int value1 ) {
	unsigned int id = getBuff(kind);	// get buffer id
	if (buffer[id].full) flush();		// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
	buffer[id].value1 = value1;
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
	unsigned int id = getBuff(kind);	// get buffer id
	if (buffer[id].full) flush();		// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
	buffer[id].value1 = value1;
	buffer[id].value2 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
	unsigned int id = getBuff(kind, lid);	// get buffer id
	if (buffer[id].full) flush();			// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	unsigned int id = getBuff(kind, lid);	// get buffer id
	if (buffer[id].full) flush();			// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
	buffer[id].value1 = value1;
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	unsigned int id = getBuff(kind, lid);	// get buffer id
	if (buffer[id].full) flush();			// write out if full
	hbiu = max(hbiu, id);

	buffer[id].full = true;
	buffer[id].state = state;
	buffer[id].value1 = value1;
	buffer[id].value2 = value2;
}