#pragma once

#include <uFuture.h>
#include <deque>
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

class WATCard {
    WATCard( const WATCard & )= delete;       // prevent copying
    WATCard &operator=( const WATCard & ) = delete;
    unsigned int balance;
  public:
    typedef Future_ISM<WATCard *> FWATCard;   // future watcard pointer
    WATCard();
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
}; // WATCard


_Task WATCardOffice {
    struct Args {
        WATCard* watcard;       // watCard, if any, create() makes this NULL
        unsigned int sid;       // student id
        unsigned int amount;    // amount to add to watCard
    };
    struct Job {                              // marshal arguments and return future
        Args args;                            // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;             // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier {
        Bank &bank;
        WATCardOffice &wco;
        unsigned int id;        // my courier id
        void main();
      public:
        Courier(Bank &bank, WATCardOffice &wco, unsigned int id);
    };
    Printer &printer;
    Bank &bank;
    unsigned int numCouriers;
    Courier **couriers;         // array of Courier pointers
    uCondition courierBench;    // bench for waiting workers
    std::deque<Job*> requests;  // queue of jobs to be done
    bool stop;                  // stop set true by destructor - halts work and ejects Couriers

    void main();
  public:
    _Event Lost {};                           // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};