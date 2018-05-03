CXX = u++					# compiler
CXXFLAGS = -g -Wall -O2 -multi -MMD -std=c++11	# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS2 = soda.o config.o printer.o bank.o parent.o watcardoffice.o groupoff.o \
vendingmachine.o nameserver.o bottlingplant.o truck.o student.o # list of object files
EXEC2 = soda

OBJECTS = ${OBJECTS2}				# all object files
DEPENDS = ${OBJECTS:.o=.d}			# substitute ".o" with ".d"
EXECS = ${EXEC2}				# all executables

#############################################################

.PHONY : all clean

all : ${EXECS}					# build all executables

${EXEC2} : ${OBJECTS2}
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f *.d *.o ${EXECS}

run :
	${MAKE} ${EXEC2}	# build the program
	@printf "\n"
	@./${EXEC2}			# run it
