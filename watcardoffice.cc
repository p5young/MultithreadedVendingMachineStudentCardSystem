#include "watcardoffice.h"


// WATCard functions -----------------------------------------------------------------------WATCARD
WATCard::WATCard() {
    balance = 0;
}

void WATCard::deposit( unsigned int amount ) {
    balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    balance -= amount;
}

unsigned int WATCard::getBalance() {
    return balance;
}

// Courier functions -----------------------------------------------------------------------COURIER

// main
void WATCardOffice::Courier::main() {
    for ( ;; ) {                                        // loop until wco.requestWork() returns NULL
        _Accept( ~Courier ) {                           // accept destructor
            break;                                      // break
        } _Else {
            Job * myjob = wco.requestWork();
          if (myjob == NULL) break;                     // WATCardOffice said stop - destructor call imminent
            unsigned int sid = myjob->args.sid;         // set student id
            unsigned int amount = myjob->args.amount;   // set amount to transfer
            if (myjob->args.watcard == NULL)            // if watCard doesn't exist, make one
                myjob->args.watcard = new WATCard();
            wco.printer.print(Printer::Courier, id, 't', sid, amount);
            bank.withdraw(sid, amount);                     // withdraw from bank
            myjob->args.watcard->deposit(amount);           // deposit onto card
            if (myjob->result.cancelled()) {                // student destroyed, delete watcard
                delete myjob->args.watcard;
            } else if (mprng(5) == 0) {                     // lose watcard
                assert(myjob->result.exception( new Lost ));
                delete myjob->args.watcard;
                wco.printer.print(Printer::Courier, id, 'L', sid);
            } else {                                        // deliver watcard
                myjob->result.delivery(myjob->args.watcard);
                wco.printer.print(Printer::Courier, id, 'T', sid, amount);
            }
            delete myjob;
        }
    } // for
    wco.printer.print(Printer::Courier, id, 'F');
} // main

// constructor
WATCardOffice::Courier::Courier(Bank &bank, WATCardOffice &wco, unsigned int id) :
    bank(bank), wco(wco), id(id)
{
    wco.printer.print(Printer::Courier, id, 'S');
}; // constructor

// WATCardOffice functions -----------------------------------------------------------WATCARDOFFICE

// main
void WATCardOffice::main() {
    for ( ;; ) {
        _Accept( requestWork ) {
        } or _Accept( create ) {
        } or _Accept( transfer ) {
        } or _Accept( ~WATCardOffice ) {                    // destructor
            stop = true;                                    // stop = true -> requestWork() returns NULL
            while (!courierBench.empty())                   // remove couriers on bench
                courierBench.signalBlock();
            for ( ;; ) {
                _Accept( requestWork ) {} _Else break;      // remove couriers calling requestWork()
            }
            break;                                          // exit main
        }
    }
} // main

// constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    printer(prt), bank(bank), numCouriers(numCouriers)
{
    couriers = new Courier*[numCouriers];               // create array of couriers
    for (unsigned int i = 0 ; i < numCouriers ; ++i)
        couriers[i] = new Courier(bank, *this, i);      // initialize array - create couriers
    stop = false;                                       // stop == false until destructor call
    printer.print(Printer::WATCardOffice, 'S');         // print start message
} // constructor

// destructor
WATCardOffice::~WATCardOffice() {
    // delete couriers
    for (unsigned int i = 0 ; i < numCouriers ; ++i) {
        delete couriers[i];
    }
    delete couriers;

    // delete jobs
    while (!requests.empty()){
        delete requests.front();
        requests.pop_front();
    }

    // print finish message
    printer.print(Printer::WATCardOffice, 'F');
} // destructor

// create a new WATCard and load it with cash
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job* job = new Job( { NULL, sid, amount} );                 // create new job
    requests.push_back(job);                                    // add it to the list of requests
    courierBench.signal();                                      // signal a courier
    printer.print(Printer::WATCardOffice, 'C', sid, amount);    // print message
    return job->result;                                         // return future
} // create

// student request to transfer funds
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    Job* job = new Job( { card, sid, amount} );                 // create job
    requests.push_back(job);                                    // add it to the list of requests
    courierBench.signal();                                      // signal a courier
    printer.print(Printer::WATCardOffice, 'T', sid, amount);    // print message
    return job->result;                                         // return future
} // transfer

WATCardOffice::Job* WATCardOffice::requestWork() {
    if (stop) return NULL;                      // stop set true in main, kill couriers
    if (requests.empty()) courierBench.wait();  // wait for work or destructor call
    if (stop) return NULL;                      // stop set true in main, kill couriers
    assert(!requests.empty());                  // ensure request exists
    Job * job = requests.front();               // get job
    requests.pop_front();                       // remove job from requests
    printer.print(Printer::WATCardOffice, 'W'); // print message
    return job;                                 // return
} // requestWork
