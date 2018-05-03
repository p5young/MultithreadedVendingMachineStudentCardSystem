#pragma once

#include "MPRNG.h"
#include "printer.h"
#include "watcardoffice.h"
#include <vector>

extern MPRNG mprng;

_Task Groupoff {
	Printer &printer;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;
	std::vector<WATCard::FWATCard> jobs;	// list of futures to be handed out & fulfilled
	// number of futures waiting to be fulfilled - incremented by giftCard() - decrimented in main()
	unsigned int n;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();			// get a future WATCard
};