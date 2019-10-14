.PHONY: all
all: mytap

.PHONY: clean
clean:
	rm *.o libmytap.a


OPT=-O2

mytap: mytap.cpp mytap.h command_line.cpp command_line.h
	g++ -c mytap.cpp command_line.cpp  -std=c++11
	ar rcs libmytap.a mytap.o command_line.o


