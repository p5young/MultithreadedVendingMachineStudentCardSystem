#pragma once

#include "printer.h"
#include "nameserver.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "vendingmachine.h"
#include "MPRNG.h"

extern MPRNG mprng;

_Task Student {
	Printer &printer;
	NameServer &nameserver;
	WATCardOffice &wco;
	Groupoff &groupoff;
	unsigned int id;
	unsigned int num;	            // number of sodas to purchase
	VendingMachine::Flavours flav;	// flavor to purchase
	WATCard::FWATCard watCard;
	WATCard::FWATCard giftCard;
	VendingMachine *vm;
    void main();
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
    ~Student();
};
