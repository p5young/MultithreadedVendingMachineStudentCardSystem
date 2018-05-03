#include "bottlingplant.h"
#include "truck.h"
#include "printer.h"

#include <iostream>
using namespace std;

//constructor
BottlingPlant::BottlingPlant(Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
    unsigned int timeBetweenShipments)
    : printer(&prt), nameServer(&nameServer), numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments) {
   
    printer->print(Printer::Kind::BottlingPlant, 'S'); 
}

//destructor
BottlingPlant::~BottlingPlant() {
    printer->print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
    //point to cargo for processing
    truckCargo = &cargo;
    //wait for processing to complete
    truckBench.wait();
    //check for shutdown
    if (shuttingDown) throw Shutdown();
}

void BottlingPlant::main() {
    Truck truck(*printer, *nameServer, *this, numVendingMachines, maxStockPerFlavour);
    for (;;) {
        //don't generate if it was just a destructor call
        if (!shuttingDown) {
            yield(timeBetweenShipments);
            int created = 0;
            for (unsigned int i = 0; i < 4; i++) {
                nextCargo[i] = mprng(maxShippedPerFlavour);
                created += nextCargo[i];
            }
            printer->print(Printer::Kind::BottlingPlant, 'G', created);
        }

        try {
            _Accept(~BottlingPlant) {
                shuttingDown = true;
            } or _Accept(getShipment) {
                printer->print(Printer::Kind::BottlingPlant, 'P');

                //truck has copied cargo pointer in now
                if (shuttingDown) { 
                    truckBench.signalBlock();
                    break;
                }

                //replace next cargo
                for (unsigned int i = 0; i < 4; i++) {
                   (*truckCargo)[i] = nextCargo[i];
                }

                //let truck know good to go
                truckBench.signalBlock();
            }
        } catch ( uMutexFailure::RendezvousFailure ) {
            cout<<"Rendezvous failure confirmed"<<endl;
            break; // is this necessary
        }
    }
}
