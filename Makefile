CPP=g++
CPPFLAGS=-Wall -O3 -std=c++11

SOURCES = src/fhgc.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: fhgc
	$(CPP) $(CPPFLAGS) src/fhgc.o -o fhgc
	$(MAKE) -C src/webgraph
	$(MAKE) -C src/faststep
	$(MAKE) -C src/benchmarks
	cd src/law+faststep; ant

fhgc: src/fhgc.o
	$(CPP) $(CPPFLAGS) src/fhgc.cpp -c -o src/fhgc.o

clean:
	rm -rf $(OBJECTS) fhgc
	$(MAKE) clean -C src/webgraph
	$(MAKE) clean -C src/faststep
