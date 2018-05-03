#include "truck.h"
#include "MPRNG.h"
#include "printer.h"
#include "bottlingplant.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include <algorithm>        // std::min

extern MPRNG mprng;

using namespace std;

//constructor
Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
    unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
    : printer(&prt), nameServer(&nameServer), plant(&plant),
    numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {

    printer->print(Printer::Kind::Truck, 'S');
}

//destructor
Truck::~Truck() {
    printer->print(Printer::Kind::Truck, 'F');
}

void Truck::main() {
    VendingMachine** machines = nameServer->getMachineList();
    for (;;) {
        try {
            //get coffee
            yield(mprng(9)+1);
            //load cargo
            plant->getShipment(cargo);
            int total = 0;
            for (unsigned int i = 0; i < 4; i++) {
                total += cargo[i];
            }
            printer->print(Printer::Kind::Truck, 'P', total);

            //no point returning to same machine if could not fulfil before
            for (unsigned int i = 0; i < numVendingMachines; i++) {
                if (total == 0) break;

                //get machine stock
                unsigned int* machineStock;
                printer->print(Printer::Kind::Truck, 'd', machines[serveIndex]->getId(), total);
                machineStock = machines[serveIndex]->inventory();
                unsigned int dif = 0;

                //load up flavours
                for (unsigned int flavInd = 0; flavInd < 4; flavInd++) {
                    unsigned int requested = maxStockPerFlavour - machineStock[flavInd];
                    int amount = min(requested, cargo[flavInd]);
                
                    //track flavours unable to fully restock 
                    if (requested > cargo[flavInd])
                        dif += requested - cargo[flavInd];
 
                    machineStock[flavInd] += amount;
                    cargo[flavInd] -= amount;
                    total -= amount;
                }
            
                if (dif > 0) //if could not fully restock machine
                    printer->print(Printer::Kind::Truck, 'U', machines[serveIndex]->getId(), dif);
                printer->print(Printer::Kind::Truck, 'D', machines[serveIndex]->getId(), total);
                machines[serveIndex]->restocked();
                serveIndex = (serveIndex + 1) % numVendingMachines;
            }
        } catch ( BottlingPlant::Shutdown ) {
            break;;
        }
    }
}
