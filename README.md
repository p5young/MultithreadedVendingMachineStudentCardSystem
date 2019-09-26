# To run this program, you must install uC++
https://github.com/pabuhr/uCPP

This simulates a group of students using their student cards to purchase soda.
These objects are run as separate threads, and use a combination of synchronous and asynchronous behavior to function smoothly.

I am posting this school assignment to my github because:
A) My partner and I received full marks
B) This assignment is no longer used, so I'm not worried about others cheating
C) It was a lot of fun to make

parent.cc - simulates the parents. They deposit money into student's student card accounts.\
bank.cc - simulates the bank. Parents can only deposit into student cards after withdrawing from bank.\
groupoff.cc - simulates the group coupon office. Gives out n gift cards to students.\
watcardoffice.cc - stores the balances for all the students' student card. Works similar to a bank (is contacted by vending machines)\
nameserver.cc - assigns names to vending machines and keeps a registry of them\
bottlingplant.cc - produces different kinds of soda\
truck.cc - delivers soda to vending machines\
vendingmachine.cc - interacts with couriers, students, nameserver, and watcardoffice\
printer.cc - receives updates from all threads about their status, prints to the terminal\
config.cc - used to alter settings of the simulation\

sodaEX - If you don't want to install uC++, I've included the compiled executable program\
Run with ./sodaEX
