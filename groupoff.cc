#include "groupoff.h"

 // main
void Groupoff::main() {
	for (unsigned int i = 0 ; i < numStudents ; ++i) {
		_Accept( ~Groupoff ) {
			return;						// destructor called - exit main
		} or _Accept( giftCard ) {
			// giftCard given out
		}
	}
	// continue handing out giftcards until n == 0
	for ( ;; ) {
		yield(groupoffDelay);	// yield
		_Accept( ~Groupoff ) {	// accept destructor
			break;
		} _Else {
			WATCard *giftcard = new WATCard();	// create giftCard
			giftcard->deposit(sodaCost);		// fill with moneys
			unsigned int winner = mprng(n-1);	// decide which future to fulfill
			if (jobs[winner].cancelled()){		// don't deliver giftCard to destroyed student
				delete giftcard;
			} else {
				jobs[winner].delivery(giftcard);					// deliver giftCard
				printer.print(Printer::Groupoff, 'D', sodaCost);	// print message
			}
			jobs.erase(jobs.begin() + winner);	// remove fulfilled future from list
			--n;								// decriment future list size
		  if (n == 0) break;					// exit if every future fulfilled
		}
	}
} // main

// constructor
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
	printer(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), jobs(numStudents)
{
	n = 0;									// initialize number of futures handed out to 0
	printer.print(Printer::Groupoff, 'S');	// print start message
}; // constructor

// destructor
Groupoff::~Groupoff() {
	printer.print(Printer::Groupoff, 'F');	// print finish message
} // destructor

// giftCard
WATCard::FWATCard Groupoff::giftCard() {
	return jobs[n++];						// return a future and increment number of futures handed out
} // giftCard