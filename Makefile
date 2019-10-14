
.PHONY: all
all: mytap sample

.PHONY: clean
clean:
	cd mytap && ${MAKE} clean
	cd sample && ${MAKE} clean

OPT=-O2

.PHONY: mytap
mytap: 
	cd mytap && OPTZ="${O0} -ggdb " CC=${CC} CXX=${CXX} ${MAKE}

sample: mytap
	cd sample && OPTZ="${O0} -ggdb " CC=${CC} CXX=${CXX} ${MAKE}


