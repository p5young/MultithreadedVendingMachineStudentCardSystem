#include <iostream>
#include <unistd.h>     // getpid
#include <uFuture.h>
#include "MPRNG.h"      // random number generator
#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

using namespace std;

MPRNG mprng;                            // random number generator has global scope

// Determines if a char* (command line argument) is a positive int (no '-')
bool legal_int(char *str) {
    while (*str)                        // while *str isn't null terminator
        if (!isdigit(*str++))           // check for non digit, and increment
            return false;
    return true;
} // legal_int

// Displays the proper usage of the program and exits
void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] << " [ config-file [ random-seed (> 0) ] ]" << endl;
    exit( EXIT_FAILURE );               // TERMINATE
} // usage

// MAIN
void uMain::main() {
    unsigned int seed = getpid();           // set default seed as process ID
    const char *configFile = "soda.config"; // set default config file as 'soda.config'

    // process command line arguments
    switch (argc) {
    case 3:
        if (legal_int(argv[2]) && atoi(argv[2]) > 0) {
            seed = atoi(argv[2]);       // set seed
        } else {
            usage(argv);                // seed not a positive int - print usage & exit
        }
        // FALL THROUGH
    case 2:
        configFile = argv[1];           // assign config file name
        // FALL THROUGH
    case 1:
        mprng.set_seed(seed);           // initialize random-number-generator seed
        break;
    default:
        usage(argv); // more than 2 command line arguments - print usage & exit
    }

    // delcare and initialize tasks and monitors
    ConfigParms parms;
    processConfigFile(configFile, parms);

    Printer printer(parms.numStudents, parms.numVendingMachines, parms.numCouriers);

    Bank bank(parms.numStudents);

    Parent parent(printer, bank, parms.numStudents, parms.parentalDelay);

    // remainder of tasks and monitors on heap to control destruction order
    WATCardOffice *wco = new WATCardOffice(printer, bank, parms.numCouriers);

    Groupoff *groupoff = new Groupoff(printer, parms.numStudents, parms.sodaCost, parms.groupoffDelay);

    NameServer* nameServer = new NameServer(printer, parms.numVendingMachines, parms.numStudents);

    VendingMachine* machines[parms.numVendingMachines];
    for (unsigned int i = 0; i < parms.numVendingMachines; i++) {
        machines[i] = new VendingMachine(printer, *nameServer, i, parms.sodaCost, parms.maxStockPerFlavour);
    }

    BottlingPlant* plant = new BottlingPlant(printer, *nameServer, parms.numVendingMachines,
        parms.maxShippedPerFlavour, parms.maxStockPerFlavour, parms.timeBetweenShipments);
    
    Student* students[parms.numStudents];
    for (unsigned int i = 0; i < parms.numStudents; i++) {
        students[i] = new Student(printer, *nameServer, *wco, *groupoff, i, parms.maxPurchases);
    }
      
    // Destroy tasks and monitors
    for (unsigned int i = 0; i < parms.numStudents; i++) {
        delete students[i];     // program runs until Students have all the soda they want
    }

    delete groupoff;
    delete plant;
    delete wco;

    for (unsigned int i = 0; i < parms.numVendingMachines; i++) {
        delete machines[i];
    }
    delete nameServer;

}
