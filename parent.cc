#include "parent.h"

extern MPRNG mprng;

// constructor
Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
	printer(prt), bank(bank)
{
	delay = parentalDelay;					// initialize delay
	this->numStudents = numStudents;		// initialize number of students
	printer.print(Printer::Parent, 'S');	// print start message
}; // constructor

// destructor
Parent::~Parent() {
	printer.print(Printer::Parent, 'F');	// print finish message
} // destructor

// main
void Parent::main() {
	for ( ;; ) {
		_Accept( ~Parent ) {				// accept destructor
			break;
		} _Else {
			yield(delay);										// yield
			unsigned int dAmt = mprng(1,3);						// determine deposit amount
			unsigned int student = mprng(0,numStudents - 1);	// determine student to give money to
			printer.print(Printer::Parent, 'D', student, dAmt);	// print message
			bank.deposit(student,dAmt);							// make the deposit
		}
	} // for
} // main