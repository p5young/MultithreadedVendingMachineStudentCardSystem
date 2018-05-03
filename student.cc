#include "student.h"
#include "vendingmachine.h"


void Student::main() {
	// for number of drinks to puchase
	for (unsigned int i = 0 ; i < num ; ++i) {
		yield(mprng(1,10));
		for ( ;; ) {
			// use giftCard
			_Select( giftCard ) {
				assert((giftCard())->getBalance() >= vm->cost());
				try {
					vm->buy(flav, *giftCard);	// attempt buy
					printer.print(Printer::Student, id, 'G', flav, (*giftCard).getBalance());
					delete giftCard;			// buy successful - delete card
					giftCard.reset();			// prevent future giftcard use
					break;						// buy successful, break for(;;) loop
				} catch ( VendingMachine::Free ) {
					printer.print(Printer::Student, id, 'a', flav, (*giftCard).getBalance());
					yield(4);
					break;
				} catch ( VendingMachine::Stock ) {
					vm = nameserver.getMachine(id);
					printer.print(Printer::Student, id, 'V', vm->getId());
				}
			// use WATCard
			} or _Select ( watCard ) {
				try {
					vm->buy(flav, (*watCard()));	// attempt buy
					printer.print(Printer::Student, id, 'B', flav, (*watCard).getBalance());
					break;
				} catch ( VendingMachine::Free ) {
					printer.print(Printer::Student, id, 'A', flav, (*watCard).getBalance());
					yield(4);						// free soda, yield
					break;
				} catch ( VendingMachine::Funds ) {
					watCard = wco.transfer(id, vm->cost() + 5, watCard);	// insufficient funds, transfer
				} catch ( VendingMachine::Stock ) {
					vm = nameserver.getMachine(id);							// out of stock - get new machine
					printer.print(Printer::Student, id, 'V', vm->getId());
				} catch ( WATCardOffice::Lost ) {
					printer.print(Printer::Student, id, 'L');
					watCard = wco.create(id, 5);							// card lost - replace
				} // try
			} // select
		} // for
	} // for
	printer.print(Printer::Student, id, 'F');	// display finish message
} // main

// constructor
Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases ) :
printer(prt), nameserver(nameServer), wco(cardOffice), groupoff(groupoff), id(id)
{
	num = mprng(1,maxPurchases);	                        // set number of bottles to purchase
	flav = static_cast<VendingMachine::Flavours>(mprng(3)); // set flavor to purchase
	printer.print(Printer::Student, id, 'S', flav, num);	// print start message
	watCard = wco.create(id, 5);							// get watCard
	giftCard = groupoff.giftCard();							// get giftCard
	vm = nameserver.getMachine(id);							// get vending machine
	printer.print(Printer::Student, id, 'V', vm->getId());	// print machine message
}; // constructor


// destructor
Student::~Student() {
	if ( watCard.available() ) {
		try {
			(void)*watCard();
			delete watCard;					// if watCard not lost, delete it
		} catch ( WATCardOffice::Lost ) {}	// if watCard lost, do nothing
	} else {
		watCard.cancel();					// cancel watCard future
	}
	if ( giftCard.available() ) {
		delete giftCard;					// delete giftCard
	} else {
		giftCard.cancel();					// cancel giftCard future
	}
}