#ifndef BOTTLINGPLANT_H
#define BOTTLINGPLANT_H

#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"

// forward declaration
_Task truck;

extern MPRNG mprng;

_Task BottlingPlant {
    Printer* printer;
    NameServer* nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;

    bool shuttingDown = false;      //flag for destructor call
    unsigned int nextCargo[4];      //stored production
    unsigned int** truckCargo;      //copy-in pointer to truck's cargo
    uCondition truckBench;          //truck waits here while loading cargo

    void main();
public:
    _Event Shutdown {};         //shutdown plant
    BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments);
    ~BottlingPlant();
    void getShipment(unsigned int cargo[]);
};

#endif //BOTTLINGPLANT_H
