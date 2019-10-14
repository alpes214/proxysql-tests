INCLUDEDIR=../mytap
LIBDIR=../mytap

.PHONY: all
all: sample

.PHONY: clean
clean:
	rm basic-t set_character_set-t

OPT=-O2

SRC=basic-t.cpp set_character_set-t.cpp

sample: basic-t set_character_set-t

basic-t:
	g++ basic-t.cpp -I$(INCLUDEDIR) -L$(LIBDIR) -std=c++11 -lmytap -o basic-t

set_character_set-t: set_character_set-t.cpp set_character_set-t.cpp
	g++ set_character_set-t.cpp -I$(INCLUDEDIR) -L$(LIBDIR) -std=c++11 `mysql_config --include --libs` -lmysqlclient -lmytap -o set_character_set-t


