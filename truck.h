#ifndef TRUCK_H
#define TRUCK_H

#include "vendingmachine.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "printer.h"

_Task Truck {
    Printer* printer;
    NameServer* nameServer;
    BottlingPlant* plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
   
    unsigned int cargo[4];          //stores truck cargo
    unsigned int serveIndex = 0;    //machine to service, note not id
    
    void main();
public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
    ~Truck();
};

#endif //TRUCK_H
