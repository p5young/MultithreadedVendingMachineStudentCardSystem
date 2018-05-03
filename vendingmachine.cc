#include "vendingmachine.h"
#include "nameserver.h"
#include <iostream>

//constructor
VendingMachine::VendingMachine(Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
    unsigned int maxStockPerFlavour) :
    printer(&prt), nameServer(&nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {

    //print starting
    prt.print(Printer::Kind::Vending, id, 'S', sodaCost);
    //register self
    nameServer.VMregister(this);
}

//destructor
VendingMachine::~VendingMachine() {
    printer->print(Printer::Kind::Vending, id, 'F');
}

void VendingMachine::buy(Flavours flavour, WATCard &card) {
    //copy in
    buyFlavour = flavour;
    wcard = &card;

    //wait for processing
    buyBench.wait();

    //process result
    switch(exceptionFlag) {
        case VM_PURCHASE:
            break;
        case VM_OUTOFSTOCK:
            _Throw Stock();
            break;
        case VM_FREE:
            _Throw Free();
            break;
        case VM_NOFUNDS:
            _Throw Funds();
            break;
        default:
            //something has gone terribly wrong
            std::cerr<<"Corrupt exceptionFlag in vendingmachine"<<std::endl;
            break;
    }
}

//retrieve internal stock for manipulation
unsigned int * VendingMachine::inventory() {
    restocking = true;
    //print start reloading by truck
    printer->print(Printer::Kind::Vending, id, 'r');
    return stock;
}

//signal restocking completion. stock safe to use
void VendingMachine::restocked() {
    //work handled in main
}

//noMutex, value is read only
unsigned int VendingMachine::cost() {
    return sodaCost;
}

//noMutex, value is read only
unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    for (;;) {
        try {
            _Accept(~VendingMachine) {
                break;
            } or _When(restocking) _Accept(restocked) {
                restocking = false;
                printer->print(Printer::Kind::Vending, id, 'R');
            } or _When(!restocking) _Accept(inventory) {

            } or _When(!restocking) _Accept(buy) {
                //check if out of stock
                if (stock[buyFlavour] == 0) {
                    exceptionFlag = VM_OUTOFSTOCK;
                //check for lucky free
                } else if (mprng(4) == 0) {
                    stock[buyFlavour] -= 1;
                    printer->print(Printer::Kind::Vending, id, 'B', buyFlavour, stock[buyFlavour]);
                    exceptionFlag = VM_FREE;
                //not free, check for balance
                } else if (wcard->getBalance() < sodaCost) {
                    exceptionFlag = VM_NOFUNDS;
                //ready for normal purchase
                } else {
                    wcard->withdraw(sodaCost);
                    stock[buyFlavour] -= 1;
                    printer->print(Printer::Kind::Vending, id, 'B', buyFlavour, stock[buyFlavour]);
                    exceptionFlag = VM_PURCHASE;
                }
                //wake customer 
                buyBench.signalBlock();
            }
        } catch ( uMutexFailure::RendezvousFailure ) {
            //exception was thrown in buy. student handles exception
            //don't need to do anything here
        }
    }
}
