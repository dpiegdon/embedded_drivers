
.PHONY: all test clean

CXXFLAGS += -std=c++17
LDFLAGS += -lboost_unit_test_framework

SOURCES=$(wildcard *.cpp *.c)
BINARIES=$(patsubst %.c,%,$(patsubst %.cpp,%,${SOURCES}))

all: clean test

test: ${BINARIES}
	for TEST in ${BINARIES}; do 	\
		echo "running $$TEST";	\
		./$$TEST;		\
	done;

clean:
	-rm -f ${BINARIES}
	
