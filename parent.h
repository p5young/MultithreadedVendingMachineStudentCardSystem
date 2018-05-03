#pragma once

#include "bank.h"
#include "printer.h"
#include "MPRNG.h"

_Task Parent {
	Printer &printer;
	Bank &bank;
	unsigned int delay;			// delay amount between deposits
	unsigned int numStudents;
    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
    ~Parent();
};