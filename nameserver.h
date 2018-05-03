#ifndef NAMESERVER_H
#define NAMESERVER_H

#include "vendingmachine.h"

_Task NameServer {
    Printer* printer;
    unsigned int numVendingMachines;
    unsigned int numStudents;

    unsigned int numRegistered = 0;     //don't start accepting machine requests until all registered
    unsigned int* studentMachineIndex;  //array to map student to their current machine index
    VendingMachine** machines;         //storage for registered vending machines 

    void main();
public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine * vendingmachine);
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();
};

#endif //NAMESERVER_H
