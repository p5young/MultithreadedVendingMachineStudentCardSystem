#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

#include "printer.h"
#include "watcardoffice.h"
#include "MPRNG.h"

extern MPRNG mprng;

//forward declaration
_Task NameServer;

_Task VendingMachine {
    //base settings
    Printer* printer;
    NameServer* nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;

    uCondition buyBench; //bench to wait on when buying

    //stock storage
    unsigned int stock[4] = { 0, 0, 0 ,0 };     //better way than hardcoding?

    //if a truck is modifying stock
    bool restocking = false;

    void main();
public:
    enum Flavours { bc = 0,     // blues black-cherry
                    cs = 1,     // classic cream-soda
                    rb = 2,     // rock root-beer
                    jl = 3 };   // jazz lime
    _Event Free {};             // free, advertisement
    _Event Funds {};            // insufficient funds
    _Event Stock {};            // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    ~VendingMachine();
    void buy( Flavours flavour, WATCard &card );
    unsigned int * inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();

private:
    //copy in values for buy call
    WATCard* wcard;
    Flavours buyFlavour;
    unsigned int exceptionFlag = 0;
    //defines for exception to throw
    #define VM_PURCHASE 0 //all clear
    #define VM_OUTOFSTOCK 1
    #define VM_FREE 2
    #define VM_NOFUNDS 3
};
    

#endif //VENDINGMACHINE_H
