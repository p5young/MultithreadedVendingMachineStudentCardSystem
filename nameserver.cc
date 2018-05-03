#include "nameserver.h"
#include "printer.h"

//constructor
NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents)
    : printer(&prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {

    prt.print(Printer::Kind::NameServer, 'S');

    //holds indeces of which machine each is using
    //note, this does not mean id of machine, they can register in random order
    studentMachineIndex = new unsigned int[numStudents];
    for (unsigned int i = 0; i < numStudents; i++) {
        studentMachineIndex[i] = i % numVendingMachines; //round robin assignment
        assert(studentMachineIndex[i] < numVendingMachines);
    }
   
    //holds machine references in order of registration
    machines = new VendingMachine*[numVendingMachines];
}

//destructor
NameServer::~NameServer() {
    delete studentMachineIndex;
    delete machines;
    printer->print(Printer::Kind::NameServer, 'F');
}

//register a machine, store pointer, increment numRegistered
void NameServer::VMregister( VendingMachine * vendingmachine ) {
    machines[numRegistered++] = vendingmachine;
    printer->print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
}

//get correct machine for student, increment index for next getMachine call from same student
VendingMachine* NameServer::getMachine( unsigned int id ) {
    using namespace std;
    assert(studentMachineIndex[id] < numVendingMachines);
    VendingMachine* machine = machines[studentMachineIndex[id]];
    studentMachineIndex[id] = (studentMachineIndex[id] + 1) % numVendingMachines;
    assert(studentMachineIndex[id] < numVendingMachines);
    printer->print(Printer::Kind::NameServer, 'N', id, machine->getId());
    if (studentMachineIndex[id] > numVendingMachines)
        studentMachineIndex[id] = 0;
    return machine;
}

//returns array of machine pointers for trucks to reload
VendingMachine** NameServer::getMachineList() {
    assert(numRegistered == numVendingMachines);

    return machines;
}

void NameServer::main() {
    for (;;) {
        //don't allow calls until all registered
        _Accept( ~NameServer ) { //don't destroy until all have registered
            break;
        } or _When(numRegistered == numVendingMachines) _Accept(getMachineList, getMachine) {
        } or _When(numRegistered < numVendingMachines) _Accept(VMregister) {
        }
    }
}
